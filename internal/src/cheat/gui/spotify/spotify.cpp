#include "pch.h"
#include "spotify.hpp"
#include <shellapi.h>

void SpotifyHandler::HandleRedirect()
{

}

bool TryParse(nlohmann::json* Json, std::string Content)
{
	try
	{
		const auto Temp = nlohmann::json::parse(Content);
		*Json = Temp;
		return true;
	}
	catch (const nlohmann::json::parse_error& Exception) {
		return false;
	}
}

std::string base64_encode(const std::string& input) {
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string encoded;
	int val = 0, bits = -6;
	const unsigned int b63 = 0x3F;

	for (const unsigned char& c : input) {
		val = (val << 8) + c;
		bits += 8;
		while (bits >= 0) {
			encoded.push_back(base64_chars[(val >> bits) & b63]);
			bits -= 6;
		}
	}

	if (bits > -6)
		encoded.push_back(base64_chars[((val << 8) >> (bits + 8)) & b63]);
	while (encoded.size() % 4)
		encoded.push_back('=');

	return encoded;
}

void SaveDataToRegistry(const std::string& keyName, const std::string& jsonData) {
	HKEY hKey;
	LONG result = RegCreateKeyEx(HKEY_CURRENT_USER, keyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
	if (result == ERROR_SUCCESS) {
		result = RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE*)jsonData.c_str(), jsonData.length() + 1);
		if (result == ERROR_SUCCESS)
			std::cout << "Data saved to registry successfully." << std::endl;
		else
			std::cerr << "Error: Failed to save data to registry." << std::endl;
		RegCloseKey(hKey);
	}
	else {
		std::cerr << "Error: Failed to create or open registry key." << std::endl;
	}
}

std::string RetrieveDataFromRegistry(const std::string& keyName) {
	HKEY hKey;
	LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, keyName.c_str(), 0, KEY_READ, &hKey);
	if (result == ERROR_SUCCESS) {
		char buffer[1024];
		DWORD bufferSize = sizeof(buffer);
		result = RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)buffer, &bufferSize);
		if (result == ERROR_SUCCESS) {
			RegCloseKey(hKey);
			return std::string(buffer);
		}
		else {
			std::cerr << "Error: Failed to retrieve data from registry." << std::endl;
		}
		RegCloseKey(hKey);
	}
	else {
		std::cerr << "Error: Failed to open registry key." << std::endl;
	}
	return "";
}

bool SpotifyHandler::Authenticate()
{
	if (Authenticated) {
		return false;
	}

	std::string keyName = "Software\\raid0\\spotify";
	const auto Cache = RetrieveDataFromRegistry(keyName);
	if (!Cache.empty())
	{
		json CacheJson = json::parse(Cache);
		RefreshToken = CacheJson["refresh_token"].get<std::string>();

		Authenticated = true;
		std::cout << "Cache found" << std::endl;

		return true;
	}

	const std::string RedirectUrl = std::string("http://localhost:2371/callback");
	const std::string Scopes = "user-library-read user-read-private user-read-recently-played user-read-recently-played user-read-playback-position playlist-read-private playlist-read-collaborative streaming app-remote-control user-read-currently-playing user-modify-playback-state user-read-playback-state";
	const std::string AuthorizationUrl = std::string("https://accounts.spotify.com/authorize?response_type=code&client_id=") + ClientID.data() + std::string("&scope=") + Scopes.data() + std::string("&redirect_uri=") + RedirectUrl.data();

	O2AuthHandler.Get("/callback", [this](const httplib::Request& Request, httplib::Response& Response) 
	{
		auto AccessToken = Request.params.find("code");
		auto Error = Request.params.find("error");

		if (Error != Request.params.end()) 
		{
			Response.set_content("Authorization error :(", "text/plain");

			ListenerOpen = false;
			O2AuthHandler.stop();
			return;
		}

		if (AccessToken == Request.params.end())
		{
			Response.set_content("Authorization unsuccessful :(", "text/plain");

			ListenerOpen = false;
			O2AuthHandler.stop();
			return;
		}
		
		Token = AccessToken->second;
		Authenticated = true;

		Response.set_content("Authorization successful!", "text/plain");
		
		auto StopThread = std::thread([this]()
		{
			Sleep(1000);
			O2AuthHandler.stop();
			ListenerOpen = false;
		});

		StopThread.detach();
	});

	ShellExecute(NULL, "open", AuthorizationUrl.data(), NULL, NULL, SW_SHOWNORMAL);

	if (!ListenerOpen && !Authenticated) 
	{
		ListenerOpen = true;
		O2AuthHandler.listen("0.0.0.0", 2371);

		DWORD StartTime = GetTickCount64();
		while (true && !INSTANCE_DESTRUCT)
		{
			if (!O2AuthHandler.is_running()) {
				break;
			}

			if ((GetTickCount64() - StartTime) > 20000) 
			{
				O2AuthHandler.stop();
				ListenerOpen = false;
				break;
			}

			Sleep(1000);
		}

		if (O2AuthHandler.is_running() || ListenerOpen) 
		{
			//AuthTimeout
			O2AuthHandler.stop();
			ListenerOpen = false;;
		}
	}

	if (Authenticated == false)
	{
		Toggled = false;

		NotificationHandlerPtr->RegisterNotification("Spotify Status", "spotify connection unsuccessful", 2, 3000);
		return false;
	}

	//AUTH TOKEN GOTTEN

	cpr::Parameters Parameters;
	Parameters.Add({ "code", Token });
	Parameters.Add({ "redirect_uri", RedirectUrl });
	Parameters.Add({ "grant_type", "authorization_code" });

	cpr::Header Header;
	Header.emplace("content-type", "application/x-www-form-urlencoded");
	Header.emplace("Authorization", "Basic " + base64_encode(ClientID + ":" + ClientSecret));

	const cpr::Response Response = cpr::Post(cpr::Url{ "https://accounts.spotify.com/api/token" }, Parameters, Header);

	if (Response.status_code != 200) 
	{
		Toggled = false;

		NotificationHandlerPtr->RegisterNotification("Spotify Status", "spotify connection unsuccessful", 2, 3000);
		return false;
	}

	json AccessTokenResponse;
	if (!TryParse(&AccessTokenResponse, Response.text)) 
	{
		Toggled = false;

		NotificationHandlerPtr->RegisterNotification("Spotify Status", "spotify connection unsuccessful", 2, 3000);
		return false;
	}

	try
	{
		json LocalData;
		LocalData["refresh_token"] = AccessTokenResponse["refresh_token"].get<std::string>();

		SaveDataToRegistry("Software\\raid0\\spotify", LocalData.dump());

		RefreshToken = AccessTokenResponse["refresh_token"].get<std::string>();

		SpotifyData = SpotifyInfo
		(
			AccessTokenResponse["refresh_token"].get<std::string>(),
			AccessTokenResponse["access_token"].get<std::string>(),
			Scopes,
			AccessTokenResponse["expires_in"].get<int>(),
			GetTickCount64() + AccessTokenResponse["expires_in"].get<int>()
		);

		NotificationHandlerPtr->RegisterNotification("Spotify Status", "spotify has been connected", 2, 3000);

		return true;
	}
	catch (const json::exception& Exc) {
		std::cerr << "JSON exception: " << Exc.what() << std::endl;
	}
	catch (const std::exception& Exc) {
		std::cerr << "Standard exception: " << Exc.what() << std::endl;
	}

	Toggled = false;
	NotificationHandlerPtr->RegisterNotification("Spotify Status", "spotify connection unsuccessful", 2, 3000);
	return false;
}

bool SpotifyHandler::RefreshLocalToken()
{
	if (!Authenticated) {
		return false;
	}

	const std::string Scopes = "user-library-read user-read-private user-read-recently-played user-read-recently-played user-read-playback-position playlist-read-private playlist-read-collaborative streaming app-remote-control user-read-currently-playing user-modify-playback-state user-read-playback-state";

	cpr::Payload Payload = 
	{
		{"grant_type", "refresh_token"},
		{"refresh_token", RefreshToken},
		{"client_id", ClientID}
	};

	cpr::Header Header;
	Header.emplace("content-type", "application/x-www-form-urlencoded");
	Header.emplace("Authorization", "Basic " + base64_encode(ClientID + ":" + ClientSecret));

	cpr::Response Response = cpr::Post(cpr::Url{ "https://accounts.spotify.com/api/token"  }, Payload, Header);

	if (Response.status_code != 200) {
		return false;
	}

	json RefreshTokenResponse;
	if (!TryParse(&RefreshTokenResponse, Response.text)) {
		return false;
	}

	try
	{
		SpotifyData = SpotifyInfo
		(
			RefreshToken,
			RefreshTokenResponse["access_token"].get<std::string>(),
			Scopes,
			RefreshTokenResponse["expires_in"].get<int>(),
			GetTickCount64() + RefreshTokenResponse["expires_in"].get<int>()
		);

		return true;
	}
	catch (const json::exception& Exc) {
		std::cerr << "JSON exception: " << Exc.what() << std::endl;
	}
	catch (const std::exception& Exc) {
		std::cerr << "Standard exception: " << Exc.what() << std::endl;
	}

	return false;
}

std::string SpotifyHandler::GetAuthToken()
{
	if (!Authenticated) {
		return {};
	}

	if ((GetTickCount64() - 10000) < SpotifyData.ExpireTime) {
		return SpotifyData.AccessToken;
	}

	if (!RefreshLocalToken()) 
	{
		//ERR
		return {};
	}

	return SpotifyData.AccessToken;
}

std::string SpotifyHandler::GetCurrentPlaybackDevice()
{
	if (!Authenticated) {
		return {};
	}

	cpr::Header Header;
	Header.emplace("Authorization", "Bearer " + GetAuthToken());

	cpr::Response Response = cpr::Get(cpr::Url{ "https://api.spotify.com/v1/me/player" }, Header);
	
	if (Response.status_code != 200) {
		return {};
	}

	if (Response.text == "EMPTY_RESPONSE") {
		return {};
	}

	json PlayBackStateResponse;
	if (!TryParse(&PlayBackStateResponse, Response.text)) {
		return {};
	}

	try {
		return PlayBackStateResponse["device"]["id"];
	}
	catch (const json::exception& Exc) {
		std::cerr << "JSON exception: " << Exc.what() << std::endl;
	}
	catch (const std::exception& Exc) {
		std::cerr << "Standard exception: " << Exc.what() << std::endl;
	}

	return {};
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GLES3/gl3.h>
bool LoadTextureFromFile(std::vector<unsigned char> Buffer, GLuint* out_texture, int* out_width, int* out_height)
{
	if (Buffer.empty()) {
		return false;
	}

	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(Buffer.data()), Buffer.size(), &image_width, &image_height, NULL, 4);

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

bool SpotifyHandler::GetCurrentlyPlayingTrackData()
{
	if (!Authenticated) {
		return false;
	}

	cpr::Header Header;
	Header.emplace("Authorization", "Bearer " + GetAuthToken());

	cpr::Response Response = cpr::Get(cpr::Url{ "https://api.spotify.com/v1/me/player/currently-playing" }, Header);

	if (Response.status_code != 200) {
		return false;
	}

	if (Response.text == "EMPTY_RESPONSE") {
		return false;
	}

	json CurrentlyPlayingTrackResponse;
	if (!TryParse(&CurrentlyPlayingTrackResponse, Response.text)) {
		return false;
	}

	try
	{
		auto TrackInfo = CurrentlyPlayingTrackResponse["item"];
		auto Album = TrackInfo["album"];
		auto AlbumImages = Album["images"];
		auto Artists = Album["artists"];
		auto LeadArtist = Artists[0];

		auto Image640 = AlbumImages[0]["url"].get<std::string>();
		auto Image300 = AlbumImages[1]["url"].get<std::string>();
		auto Image64 = AlbumImages[2]["url"].get<std::string>();

		auto AlbumName = Album["name"].get<std::string>();
		auto TrackName = TrackInfo["name"].get<std::string>();
		auto ArtistName = LeadArtist["name"].get<std::string>();
		auto ID = TrackInfo["id"].get<std::string>();
		auto Duration = TrackInfo["duration_ms"].get<int>();
		auto Progress = CurrentlyPlayingTrackResponse["progress_ms"].get<int>();

		this->ArtistName = ArtistName;
		this->AlbumName = AlbumName;
		this->SongName = TrackName;
		this->ImageUrl = Image640;
		this->Progress = Progress;
		this->Duration = Duration;

		static auto PreviousName = std::string("");

		if (TrackName != PreviousName)
		{
			cpr::Response ImageResponse = cpr::Get(cpr::Url{ Image64.data() });

			if (ImageResponse.status_code != 200) {
				return false;
			}

			ImageBuffer = std::vector<unsigned char>(ImageResponse.text.begin(), ImageResponse.text.end());

			PreviousName = TrackName;

			std::cout << "Image buffer updated" << std::endl;
		}

		return true;
	}
	catch (const json::exception& Exc) {
		std::cerr << "JSON exception: " << Exc.what() << std::endl;
	}
	catch (const std::exception& Exc) {
		std::cerr << "Standard exception: " << Exc.what() << std::endl;
	}

	return false;
}

void truncateTextIfNecessary(std::string& text, const ImVec2& maxSize) {
	// Calculate the size of the text
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

	// Check if the text width exceeds the maximum allowed width
	if (textSize.x > maxSize.x) {
		// Calculate the width of "..." text
		ImVec2 ellipsisSize = ImGui::CalcTextSize("...");

		// Calculate the remaining width for the original text
		float remainingWidth = maxSize.x - ellipsisSize.x;

		// Find the position in the string where the text should be truncated
		int positionToTruncate = 0;
		float widthSoFar = 0.0f;
		for (int i = 0; i < text.size(); ++i) {
			float charWidth = ImGui::CalcTextSize(text.substr(i, 1).c_str()).x;
			if (widthSoFar + charWidth <= remainingWidth) {
				widthSoFar += charWidth;
				positionToTruncate++;
			}
			else {
				break;
			}
		}

		// Truncate the text and add "..."
		text = text.substr(0, positionToTruncate) + "...";
	}
}

void SpotifyHandler::Render()
{
	struct PlayerAnimations
	{
		std::string PreviousName;

		float Alpha = 0.F;
		float Progress = 0.F;
	};

	static std::map<ImGuiID, PlayerAnimations> AnimationMap;

	const int AnimationID = 91921928;

	auto AnimationHandle = AnimationMap.find(AnimationID);
	if (AnimationHandle == AnimationMap.end())
	{
		AnimationMap.insert({ AnimationID, PlayerAnimations() });
		AnimationHandle = AnimationMap.find(AnimationID);
	}

	if (!Toggled || !IN_GAME)
	{
		AnimationHandle->second.Alpha = 0.F;
		return;
	}

	if (!MusicFound) 
	{
		AnimationHandle->second.Alpha = 0.F;
		return;
	}

	RECT WindowRect;
	GetClientRect(WINDOW_HANDLE, &WindowRect);

	const int WindowWidth = WindowRect.right - WindowRect.left;
	const int WindowHeight = WindowRect.bottom - WindowRect.top;

	ImGui::SetNextWindowSize(ImVec2(232.F, 60.F));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.F);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.F, 0.F));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Color("0F0F0F").SetAlpha(255.F * AnimationHandle->second.Alpha).GetColorU32());
	ImGui::PushStyleColor(ImGuiCol_WindowShadow, Color("0F0F0F").SetAlpha(255.F * AnimationHandle->second.Alpha).GetColorU32());
	ImGui::PushStyleColor(ImGuiCol_Border, Color("141415").SetAlpha(255.F * AnimationHandle->second.Alpha).GetColorU32());

	auto Style = ImGui::GetStyle();
//	static float PreviousWindowShadowSize = Style.WindowShadowSize;

	Style.WindowShadowSize = 0.F;
	
	static auto PreviousSong = std::string();
	static int ClearCounter = 0;

	static GLuint CoverImageTexture;
	if (SongName != PreviousSong)
	{
		//AnimationHandle->second.Alpha = ImLerp(AnimationHandle->second.Alpha, 0.F, ImGui::GetIO().DeltaTime * 2.F);
		PreviousSong = SongName;
		AnimationHandle->second.Progress = 0.F;
		ClearCounter = 0;
	}

	AnimationHandle->second.Progress = ImLerp(AnimationHandle->second.Progress, ((float)Progress / (float)Duration), ImGui::GetIO().DeltaTime * 7.F);
	bool ChangeAlpha = true;
	if (ClearCounter < 1200) 
	{
		glDeleteTextures(1, &CoverImageTexture);
		CoverImageTexture = NULL;
		LoadTextureFromFile(ImageBuffer, &CoverImageTexture, &TextureWidth, &TextureHeight);

		ClearCounter++;
		//AnimationHandle->second.Alpha = ImLerp(AnimationHandle->second.Alpha, 0.F, ImGui::GetIO().DeltaTime * 2.F);
		AnimationHandle->second.Progress = ImLerp(AnimationHandle->second.Progress, ((float)Progress / (float)Duration), ImGui::GetIO().DeltaTime * 7.F);
		//return;

		//ChangeAlpha = false;
	}

	AnimationHandle->second.Progress = ImLerp(AnimationHandle->second.Progress, ((float)Progress / (float)Duration), ImGui::GetIO().DeltaTime * 7.F);

	if (Duration - Progress <= 2500) {
		//AnimationHandle->second.Alpha = ImLerp(AnimationHandle->second.Alpha, 0.F, ImGui::GetIO().DeltaTime * 2.F);
	}
	else if (ChangeAlpha) {
		AnimationHandle->second.Alpha = ImLerp(AnimationHandle->second.Alpha, 1.F, ImGui::GetIO().DeltaTime * 2.F);
	}

	ImGui::SetNextWindowBgAlpha(AnimationHandle->second.Alpha);

	ImGui::Begin("Spotify Player", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	{
		const auto DrawList = ImGui::GetWindowDrawList();
		const auto WindowBounds = ImRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize());

		const auto ImageBounds = ImRect(WindowBounds.Min + ImVec2(6.F, 6.F), WindowBounds.Min + ImVec2(48.F + 6.F, 48.F + 6.F));

		DrawList->AddImageRounded((void*)(intptr_t)CoverImageTexture, ImageBounds.Min, ImageBounds.Max, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImGui::GetColorU32(ImVec4(1.F, 1.F, 1.F, 1.F * AnimationHandle->second.Alpha)), 4.F);

		const auto SongNamePos = ImVec2(ImageBounds.Max.x + 6.F, WindowBounds.Min.y + (11.F - 3.F));
		const auto ArtistNamePos = ImVec2(ImageBounds.Max.x + 6.F, WindowBounds.Min.y + (27.F - 3.F));

		auto DescriptionText = std::string("by ") + ArtistName + ", on " + AlbumName;

		AnimationHandle->second.Progress = ImLerp(AnimationHandle->second.Progress, ((float)Progress / (float)Duration), ImGui::GetIO().DeltaTime * 7.F);

		const auto BoundingBoxProgress = ImRect(WindowBounds.Min + ImVec2(60.F, 44.F), WindowBounds.Min + ImVec2(221.F, 49.F));
		const auto BoundingBoxProgressShaded = ImRect(WindowBounds.Min + ImVec2(60.F, 44.F), WindowBounds.Min + ImVec2(60.F + std::clamp(((221.F - 60.F) * AnimationHandle->second.Progress), BoundingBoxProgress.GetHeight(), BoundingBoxProgress.GetWidth()), 49.F));


		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[8]);

		auto SongNameText = SongName;
		truncateTextIfNecessary(SongNameText, ImVec2(BoundingBoxProgress.GetWidth(), 0));

		DrawList->AddText(SongNamePos, Color("FFFFFF").SetAlpha(255.F * AnimationHandle->second.Alpha), SongNameText.data());
		ImGui::PopFont();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[9]);

		truncateTextIfNecessary(DescriptionText, ImVec2(BoundingBoxProgress.GetWidth(), 0));
		DrawList->AddText(ArtistNamePos, Color("444751").SetAlpha(255.F * AnimationHandle->second.Alpha), DescriptionText.data());
		ImGui::PopFont();

		//DrawList->AddShadowRect(BoundingBoxProgress.Min, BoundingBoxProgress.Max, Color("444751").SetAlpha((255 * AnimationHandle->second.Alpha) * 0.35), 15.F, {0.F, 0.F}, 0, 5.F);
		DrawList->AddRectFilled(BoundingBoxProgress.Min, BoundingBoxProgress.Max, Color("111112").SetAlpha(255.F * AnimationHandle->second.Alpha), 5.F);
		DrawList->AddRectFilled(BoundingBoxProgressShaded.Min, BoundingBoxProgressShaded.Max, Color("7978A0").SetAlpha(255.F * AnimationHandle->second.Alpha), 5.F);
	}
	ImGui::End();

//	Style.WindowShadowSize = PreviousWindowShadowSize;

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
}

void SpotifyHandler::InfoThread()
{
	if (!Toggled) {
		return;
	}

	if (!Authenticated && !O2AuthHandler.is_running())
	{
		if (!Authenticate()) {
			NotificationHandlerPtr->RegisterNotification("Spotify Status", "spotify connection unsuccessful", 2, 3000);
		}

		if (!RefreshLocalToken()) {
			NotificationHandlerPtr->RegisterNotification("Spotify Status", "spotify token refresh failed", 2, 3000);
		}
	}

	if (O2AuthHandler.is_running()) {
		return;
	}

	if (!Authenticated) {
		return;
	}

	if (!GetCurrentlyPlayingTrackData()) 
	{
		MusicFound = false;
		return;
	}

	MusicFound = true;
}

SpotifyHandler* SpotifyHandlerPtr = new SpotifyHandler;