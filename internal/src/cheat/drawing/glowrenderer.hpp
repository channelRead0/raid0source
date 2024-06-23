#pragma once
#include <GL/gl.h>

#include "src/sdk/game/minecraft/framebuffer.hpp"
#include "src/sdk/game/minecraft/glstatemanager.hpp"
#include "src/sdk/game/minecraft/frustrum.hpp"
#include "src/sdk/game/minecraft/openglhelper.hpp"
#include "src/sdk/game/minecraft/player.hpp"
#include "src/sdk/game/minecraft/scaledresolution.hpp"
#include <cheat/hook/javahook.hpp>

struct Skeleton
{
	Vector3<float> Head;
	Vector3<float> RightArm;
	Vector3<float> LeftArm;
	Vector3<float> RightLeg;
	Vector3<float> LeftLeg;
};

class GlowRenderer
{
private:
	GLuint ProgramId = 0;
	GLuint FragmentShader = 0;
	GLuint VertexShader = 0;

	std::unordered_map<std::string, GLint> UniformsMap;

	std::shared_ptr<Framebuffer> FramebufferVertical = nullptr;
	std::shared_ptr<Framebuffer> FramebufferHorizontal = nullptr;


	bool Setup = false;

	GLuint CreateShader(JNIEnv* Env, const std::string& ShaderS, GLenum Type)
	{
		auto ARBShaderObjectsClass = GameClass(Env, "org.lwjgl.opengl.ARBShaderObjects"); 
		int Shader = ARBShaderObjectsClass.CallIntMethod(Env, ARBShaderObjectsClass.GetClass(), "glCreateShaderObjectARB", Type);

		const auto ShaderSource = Env->NewStringUTF(ShaderS.c_str());

		ARBShaderObjectsClass.CallVoidMethod(Env, ARBShaderObjectsClass.GetClass(), "glShaderSourceARB", Shader, ShaderSource);
		ARBShaderObjectsClass.CallVoidMethod(Env, ARBShaderObjectsClass.GetClass(), "glCompileShaderARB", Shader);

		Env->DeleteLocalRef(ShaderSource);

		return Shader;
	}


	void setUniform1i(JNIEnv* env, jint location, jint v0) {
		// Convert C-style string to Java string
		jclass gl20Class = GameClass(env, "org.lwjgl.opengl.GL20").GetClass();
		if (gl20Class == nullptr) {
			std::cerr << "Cannot find GL20 class!" << std::endl;
			return;
		}

		// Get the glUniform1i method ID
		jmethodID uniform1iMethod = env->GetStaticMethodID(gl20Class, "glUniform1i", "(II)V");
		if (uniform1iMethod == nullptr) {
			std::cerr << "Cannot find glUniform1i method!" << std::endl;
			return;
		}

		// Call glUniform1i
		env->CallStaticVoidMethod(gl20Class, uniform1iMethod, location, v0);
	}

	void setUniform1f(JNIEnv* env, jint location, jfloat v0) {
		// Convert C-style string to Java string
		jclass gl20Class = GameClass(env, "org.lwjgl.opengl.GL20").GetClass();
		if (gl20Class == nullptr) {
			std::cerr << "Cannot find GL20 class!" << std::endl;
			return;
		}

		// Get the glUniform1i method ID
		jmethodID uniform1iMethod = env->GetStaticMethodID(gl20Class, "glUniform1f", "(IF)V");
		if (uniform1iMethod == nullptr) {
			std::cerr << "Cannot find glUniform1f method!" << std::endl;
			return;
		}

		// Call glUniform1i
		env->CallStaticVoidMethod(gl20Class, uniform1iMethod, location, v0);
	}

	void setUniform2f(JNIEnv* env, jint location, float v1, float v2) {
		// Convert C-style string to Java string
		jclass gl20Class = GameClass(env, "org.lwjgl.opengl.GL20").GetClass();
		if (gl20Class == nullptr) {
			std::cerr << "Cannot find GL20 class!" << std::endl;
			return;
		}

		// Get the glUniform1i method ID
		jmethodID methodID = env->GetStaticMethodID(gl20Class, "glUniform2f", "(IFF)V");
		if (methodID == nullptr) {
			std::cerr << "Cannot find glUniform2f method!" << std::endl;
			return;
		}

		// Call glUniform1i
		env->CallStaticVoidMethod(gl20Class, methodID, location, v1, v2);
	}

	void setUniform3f(JNIEnv* env, jint location, float v1, float v2, float v3) {
		// Convert C-style string to Java string
		jclass gl20Class = GameClass(env, "org.lwjgl.opengl.GL20").GetClass();
		if (gl20Class == nullptr) {
			std::cerr << "Cannot find GL20 class!" << std::endl;
			return;
		}

		// Get the glUniform1i method ID
		jmethodID methodID = env->GetStaticMethodID(gl20Class, "glUniform3f", "(IFFF)V");
		if (methodID == nullptr) {
			std::cerr << "Cannot find glUniform3f method!" << std::endl;
			return;
		}

		// Call glUniform1i
		env->CallStaticVoidMethod(gl20Class, methodID, location, v1, v2, v3);
	}

	void UpdateUniforms(JNIEnv* Env)
	{
		/*static auto Minecraft = ::Minecraft(Env);

		glUniform1i(UniformsMap.at("texture"), 0);
		glUniform1i(UniformsMap.at("texture2"), 8);

		float texelWidth = 1.0f / Minecraft.GetDisplayWidth(Env);
		float texelHeight = 1.0f / Minecraft.GetDisplayHeight(Env);
		glUniform2f(UniformsMap.at("texelSize"), texelWidth, texelHeight);

		float alpha = 3.0f / 255.0f * 255.F;
		glUniform1f(UniformsMap.at("alpha"), alpha);
		glUniform1i(UniformsMap.at("radius"), 50.F);
		glUniform3f(UniformsMap.at("color"), 1.F, 1.F, 1.F);*/
		/*static auto Minecraft = ::Minecraft(Env);

		setUniform1i(Env, UniformsMap.at("texture"), 0);
		setUniform1i(Env, UniformsMap.at("texture2"), 8);

		float texelWidth = 1.0f / Minecraft.GetDisplayWidth(Env);
		float texelHeight = 1.0f / Minecraft.GetDisplayHeight(Env);
		setUniform2f(Env, UniformsMap.at("texelSize"), texelWidth, texelHeight);

		float alpha = 3.0f / 255.0f * 255.F;
		setUniform1f(Env, UniformsMap.at("alpha"), alpha);
		setUniform1i(Env, UniformsMap.at("radius"), 50.F);
		setUniform3f(Env, UniformsMap.at("color"), 1.F, 1.F, 1.F);*/
	}

	void DrawFrameBuffer(JNIEnv* Env, std::shared_ptr<Framebuffer> Framebuffer, std::shared_ptr<ScaledResolution> ScaledResolution)
	{
		glBindTexture(GL_TEXTURE_2D, Framebuffer->FrameBufferTexture(Env));
		glBegin(GL_QUADS);
		glTexCoord2d(0, 1);
		glVertex2d(0, 0);
		glTexCoord2d(0, 0);
		glVertex2d(0, ScaledResolution->GetScaledHeight(Env));
		glTexCoord2d(1, 0);
		glVertex2d(ScaledResolution->GetScaledWidth(Env), ScaledResolution->GetScaledHeight(Env));
		glTexCoord2d(1, 1);
		glVertex2d(ScaledResolution->GetScaledWidth(Env), 0);
		glEnd();
	}
	
	bool IsInView(JNIEnv* Env, std::shared_ptr<Player> Player)
	{
		static auto Minecraft = ::Minecraft(Env);

		auto Frustrum = ::FrustRum(Env);
		const auto RenderViewEntity = Minecraft.GetRenderViewEntityPtr(Env);

		Frustrum.SetPosition(Env, RenderViewEntity->GetPos(Env));

		return Frustrum.IsBoundingBoxInFrustum(Env, Player->GetBoundingBox(Env));
	}

	std::vector<std::shared_ptr<Player>> GetLivingPlayers(JNIEnv* Env)
	{
		static auto Minecraft = ::Minecraft(Env);
		auto Player = Minecraft.GetThePlayerPtr(Env);
		auto World = Minecraft.GetTheWorldPtr(Env);

		std::vector<std::shared_ptr<::Player>> Playerss;
		for (auto& Plyr : World->GetPlayers(Env))
		{
			if (Env->IsSameObject(Plyr->GetInstanceObject(), Player->GetInstanceObject())) {
				continue;
			}

			if (!IsInView(Env, Plyr)) {
				continue;
			}

			if (Plyr->IsInvisible(Env)) {
				continue;
			}

			if (Plyr->IsDead(Env)) {
				continue;
			}

			Playerss.push_back(Plyr);
		}

		return Playerss;
	}

	std::shared_ptr<Framebuffer> SetupFramebuffer(JNIEnv* Env, std::shared_ptr<Framebuffer> Framebuffer)
	{
		if (Framebuffer != nullptr && Framebuffer->IsValid(Env)) {
			Framebuffer->DeleteFrameBuffer(Env);
		}

		static auto Minecraft = ::Minecraft(Env);

		Framebuffer = std::make_shared<::Framebuffer>(Env, Minecraft.GetDisplayWidth(Env), Minecraft.GetDisplayHeight(Env), false);
		return Framebuffer;
	}
public:
	GlowRenderer();
	void SetupRenderer(JNIEnv* Env);
	void Render(JNIEnv* Env, float PartialTicks);

	static void SetRotationAnglesDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};

extern std::shared_ptr<GlowRenderer> GlowRendererPtr;