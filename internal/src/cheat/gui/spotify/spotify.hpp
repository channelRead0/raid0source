#pragma once
#include "httplib.h"
#include <cpr/cpr.h>
#include "nlohmann/json.hpp"
#include <src/cheat/gui/notifications/notifications.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <ext/imgui/custom/custom.hpp>

//#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/GLU.h> 

#define POPPINS_MEDIUM 8;
#define POPPINS_REGULAR 9;
#define NOTIFICATION_ICON 10;

using namespace nlohmann;

struct SpotifyInfo
{
	std::string RefreshToken;
	std::string AccessToken;
	std::string Scopes;
	int ExpireDuration;
	DWORD ExpireTime;

	/*SpotifyInfo()
	{
		this->RefreshToken = std::string();
		this->AccessToken = std::string();
		this->Scopes = std::string();
		this->ExpireDuration = 0;
		this->ExpireTime = 0;
	};
	SpotifyInfo(std::string& RefreshToken, std::string& AccessToken, std::string& Scopes, int ExpireDuration)
	{
		this->RefreshToken = RefreshToken;
		this->AccessToken = AccessToken;
		this->Scopes = Scopes;
		this->ExpireDuration = ExpireDuration;
		this->ExpireTime = GetTickCount64() + ExpireDuration;
	}*/
};

struct SongData
{
	std::string ImageUrl640px;
	std::string ImageUrl300px;
	std::string ImageUrl64px;

	std::string Name;
	std::string Artist;
	std::string Album;
	std::string TrackID;

	int DurationMs;
	int ProgressMs;
};

class SpotifyHandler
{
private:
	std::string Token;
	std::string RefreshToken;

	bool ListenerOpen = false;
	bool Authenticated = false;
	bool MusicFound = false;

	httplib::Server O2AuthHandler;

	SpotifyInfo SpotifyData;
	SongData CurrentlyPlaying;

	std::string ArtistName;
	std::string AlbumName;
	std::string SongName;
	std::string ImageUrl;

	std::vector<unsigned char> ImageBuffer;

	int TextureHeight;
	int TextureWidth;
	GLuint ImageTexture;
	
	int Duration;
	int Progress;

	static void HandleRedirect();
public:
	inline static bool Toggled = false;
	inline static std::string ClientID = "4ff7a39403ee486d949c44a4728a62b6";/*"4ff7a39403ee486d949c44a4728a62b6";*//*"f9d1ace4876d44889bf3f8c407f446a5";*///"793cb4d2a1e149f0a446bca73050d2d0";(merges)//"4ff7a39403ee486d949c44a4728a62b6";(busy)
	inline static std::string ClientSecret = "48a33eb87b544eaca04aefd775c42b5a";/*"48a33eb87b544eaca04aefd775c42b5a";*//*"834cf705517a492285a0b59b39018e1c";*///"5c8c7f7aeb164944b262cfac0f3cf4d0";(merges)//"48a33eb87b544eaca04aefd775c42b5a";(busy)

	SpotifyHandler() {}
	bool Authenticate();
	bool RefreshLocalToken();

	std::string GetAuthToken();
	std::string GetCurrentPlaybackDevice();

	bool GetCurrentlyPlayingTrackData();
	void Render();
	void InfoThread();
};

extern SpotifyHandler* SpotifyHandlerPtr;