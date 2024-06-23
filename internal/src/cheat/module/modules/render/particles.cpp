#include "pch.h"
#include "particles.hpp"

#include <sdk/game/minecraft/glstatemanager.hpp>
#include <sdk/game/minecraft/activerenderinfo.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

std::size_t hashCode(Color A) {
    // Combine the RGB values into a single hash code
    std::size_t h1 = std::hash<int>{}(A.R);
    std::size_t h2 = std::hash<int>{}(A.G);
    std::size_t h3 = std::hash<int>{}(A.B);
    return h1 ^ (h2 << 1) ^ (h3 << 2); // Combine hashes
}

void drawCircle(float x, float y, float radius) {
	int num_segments = 100; // Number of segments to approximate the circle
	glBegin(GL_POLYGON);
	for (int i = 0; i < num_segments; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments); // Get the current angle
		float dx = radius * cosf(theta); // Calculate the x component
		float dy = radius * sinf(theta); // Calculate the y component
		glVertex2f(x + dx, y + dy); // Output vertex
	}
	glEnd();
}

#include "src/cheat/drawing/glowrenderer.hpp"

void Particles::OnRender(JNIEnv* Env)
{
}

void Particles::OnRenderWorldPass(JNIEnv* Env, int Pass, float PartialTicks, long FinishTimeNano)
{

	GlowRendererPtr->SetupRenderer(Env);
	GlowRendererPtr->Render(Env, PartialTicks);

}


void maintainVectorSize(std::vector<Vector3<double>>& vec, int maxSize) {
	while (vec.size() > maxSize) {
		vec.erase(vec.begin());
	}
}

void Particles::Run(JNIEnv* Env)
{
	//GameClasses ThreadClasses;
	//if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
	//	return;
	//}

	//const auto& LocalPlayer = ThreadClasses.Player;
	//const auto& World = ThreadClasses.World;
	//const auto& Minecraft = ThreadClasses.Minecraft;

	//if (LocalPlayer->GetPos(Env).X == LocalPlayer->GetPreviousPos(Env).X && LocalPlayer->GetPos(Env).Y == LocalPlayer->GetPreviousPos(Env).Y && LocalPlayer->GetPos(Env).Z == LocalPlayer->GetPreviousPos(Env).Z) {
	//	return;
	//}

	//std::lock_guard<std::mutex> Lock(PLock); // Lock the mutex
	//PositionRecords.push_back(LocalPlayer->GetPos(Env));
	//maintainVectorSize(PositionRecords, 200);
 // //  std::cout << PositionRecords.size() << std::endl;
}
