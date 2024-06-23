#include "pch.h"
#include "glowrenderer.hpp"
#include <GLES3/gl3.h>
#include <sdk/internal/register/envregister.hpp>

GlowRenderer::GlowRenderer()
{
 
}

std::unordered_map<std::shared_ptr<Player>, Skeleton> SkeletonMap = {};
jint JNICALL CreateShaderJNI(JNIEnv* env, jstring glsl, jint type) {
    // Get the ARBShaderObjects class and its methods
    auto ARBShaderObjectsClass = GameClass(env, "org.lwjgl.opengl.ARBShaderObjects");
    jclass shaderObjectsClass = ARBShaderObjectsClass.GetClass();
    if (shaderObjectsClass == nullptr) {
        std::cerr << "Cannot find ARBShaderObjects class!" << std::endl;
        return 0;
    }

    // Get the glCreateShaderObjectARB method ID
    jmethodID createShaderMethod = env->GetStaticMethodID(shaderObjectsClass, "glCreateShaderObjectARB", "(I)I");
    if (createShaderMethod == nullptr) {
        std::cerr << "Cannot find glCreateShaderObjectARB method!" << std::endl;
        return 0;
    }

    // Get the glShaderSourceARB method ID
    jmethodID shaderSourceMethod = env->GetStaticMethodID(shaderObjectsClass, "glShaderSourceARB", "(ILjava/lang/CharSequence;)V");
    if (shaderSourceMethod == nullptr) {
        std::cerr << "Cannot find glShaderSourceARB method!" << std::endl;
        return 0;
    }

    // Get the glCompileShaderARB method ID
    jmethodID compileShaderMethod = env->GetStaticMethodID(shaderObjectsClass, "glCompileShaderARB", "(I)V");
    if (compileShaderMethod == nullptr) {
        std::cerr << "Cannot find glCompileShaderARB method!" << std::endl;
        return 0;
    }

    // Call glCreateShaderObjectARB
    jint shader = env->CallStaticIntMethod(shaderObjectsClass, createShaderMethod, type);

    // Call glShaderSourceARB
    env->CallStaticVoidMethod(shaderObjectsClass, shaderSourceMethod, shader, glsl);

    // Call glCompileShaderARB
    env->CallStaticVoidMethod(shaderObjectsClass, compileShaderMethod, shader);

    return shader;
}

jint getUniformLocation(JNIEnv* env, jint program, const char* name) {
    // Convert C-style string to Java string
    jstring javaName = env->NewStringUTF(name);
    // Find the class that contains the static method
    jclass cls = GameClass(env, "org.lwjgl.opengl.GL20").GetClass();
    // Get the method ID of glGetUniformLocation
    jmethodID methodID = env->GetStaticMethodID(cls, "glGetUniformLocation", "(ILjava/lang/CharSequence;)I");
    // Call the static method
    jint location = env->CallStaticIntMethod(cls, methodID, program, javaName);

    // Release the Java string
    env->DeleteLocalRef(javaName);

    return location;
}

void GlowRenderer::SetupRenderer(JNIEnv* Env)
{
    if (Setup) {
        return;
    }

    
   
}
void checkGLError(const char* location) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL Error (%s): %d\n", location, error);
    }
}
void GlowRenderer::Render(JNIEnv* Env, float PartialTicks)
{
    return;
    GameClasses ThreadClasses;
    if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
    {
        return;
    }

    const auto& LocalPlayer = ThreadClasses.Player;
    const auto& World = ThreadClasses.World;
    const auto& Minecraft = ThreadClasses.Minecraft;

    auto Render = [](Vector3<double> Pos) -> void
    {
        glRotatef(Pos.X * 57.29578f, 1.0f, 0.0f, 0.0f);
        glRotatef(Pos.Y * 57.29578f, 0.0f, 1.0f, 0.0f);
        glRotatef(Pos.Z * 57.29578f, 0.0f, 0.0f, 1.0f);
    };

    auto Interpolate = [](float Current, float Old, float Scale) -> float
    {
        return Old + (Current - Old) * Scale;
    };

    glDisable(2848);
    glDisable(2929);
    glDisable(3553);
    glEnable(2903);
    glDepthMask(true);

    glColor4f(1.F, 1.F, 1.F, 1.0f);

    const auto& Timer = Minecraft->GetTimerPtr(Env);
    const auto& RenderManager = Minecraft->GetRenderManagerPtr(Env);
    const auto RenderPos = RenderManager->GetRenderPos(Env);

    for (auto& [Player, Model] : SkeletonMap)
    {
        const auto Position = Player->GetPos(Env);
        const auto IsSneaking = Player->IsSneaking(Env);
        const auto PositionLastTick = Player->GetLastTickPos(Env);
        const auto RotationYawHead = Player->GetRotationYawHead(Env);
        const auto PrevYawOffset = Player->GetPrevRenderYawOffset(Env);
        const auto RenderYawOffset = Player->GetRenderYawOffset(Env);

        glPushMatrix();
        glLineWidth(1.F);
        glTranslated
        (
            Interpolate(Position.X, PositionLastTick.X, PartialTicks) - RenderPos.X,
            Interpolate(Position.Y, PositionLastTick.Y, PartialTicks) - RenderPos.Y,
            Interpolate(Position.Z, PositionLastTick.Z, PartialTicks) - RenderPos.Z
        );

        auto BodyYawOffset = PrevYawOffset + (RenderYawOffset - PrevYawOffset) * Timer->GetRenderPartialTicks(Env);
        glRotatef(-BodyYawOffset, 0.0f, 1.0f, 0.0f);
        glTranslated(0.0, 0.0, IsSneaking ? - 0.235 : 0.0);
        auto LegHeight = IsSneaking ? 0.6f : 0.75f;
        glPushMatrix();
        glTranslated(-0.125, LegHeight, 0.0);
        Render(Model.RightLeg);
        glBegin(3);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, (-LegHeight), 0.0);
        glEnd();
        glPopMatrix();
        glPushMatrix();
        glTranslated(0.125, LegHeight, 0.0);
        Render(Model.LeftLeg);
        glBegin(3);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, (-LegHeight), 0.0);
        glEnd();
        glPopMatrix();
        glTranslated(0.0, 0.0, (IsSneaking) ? 0.25 : 0.0);
        glPushMatrix();
        glTranslated(0.0, (IsSneaking) ? - 0.05 : 0.0, (IsSneaking) ? - 0.01725 : 0.0);
        glPushMatrix();
        glTranslated(-0.375, LegHeight + 0.55, 0.0);
        Render(Model.RightArm);
        glBegin(3);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, -0.5, 0.0);
        glEnd();
        glPopMatrix();
        glPushMatrix();
        glTranslated(0.375, LegHeight + 0.55, 0.0);
        Render(Model.LeftArm);
        glBegin(3);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, -0.5, 0.0);
        glEnd();
        glPopMatrix();
        glRotatef(BodyYawOffset - RotationYawHead, 0.0f, 1.0f, 0.0f);
        glPushMatrix();
        glTranslated(0.0, LegHeight + 0.55, 0.0);
        glRotatef(Model.Head.X * 57.29578f, 1.0f, 0.0f, 0.0f);
        glBegin(3);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.3, 0.0);
        glEnd();
        glPopMatrix();
        glPopMatrix();
        glRotatef((IsSneaking) ? 25.0f : 0.0f, 1.0f, 0.0f, 0.0f);
        glTranslated(0.0, (IsSneaking) ? - 0.16175 : 0.0, (IsSneaking) ? - 0.48025 : 0.0);
        glPushMatrix();
        glTranslated(0.0, LegHeight, 0.0);
        glBegin(3);
        glVertex3d(-0.125, 0.0, 0.0);
        glVertex3d(0.125, 0.0, 0.0);
        glEnd();
        glPopMatrix();
        glPushMatrix();
        glTranslated(0.0, LegHeight, 0.0);
        glBegin(3);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.55, 0.0);
        glEnd();
        glPopMatrix();
        glPushMatrix();
        glTranslated(0.0, LegHeight + 0.55, 0.0);
        glBegin(3);
        glVertex3d(-0.375, 0.0, 0.0);
        glVertex3d(0.375, 0.0, 0.0);
        glEnd();
        glPopMatrix();
        glPopMatrix();
    }
    glEnable(2848);
    glEnable(2929);
    glEnable(3553);
    glDisable(2903);
   // glColor4f(-1.F, -1.F, -1.F, -1.0f);
    glDepthMask(false);
   // if (!Setup)
   // {
   //     glUseProgram(0);
   //     auto OpenGlHelper = ::OpenGlHelper(Env);

   //     if (OpenGlHelper.ArbShaders(Env)) {
   //         std::cout << "ARB SHADERS!!!!!!!!!!!: " << OpenGlHelper.ArbShaders(Env) << std::endl;
   //     }
   //     else {

   //         std::cout << "NOOOOOOOOOOOOOOOOOOOO ARB SHADERS!!!!!!!!!!!: " << OpenGlHelper.ArbShaders(Env) << std::endl;
   //     }

   //     // Vertex hader source code
   //     const std::string VertexShaderSource =
   //         R"(
   //     void main() {
   //         gl_TexCoord[0] = gl_MultiTexCoord0;
   //         gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   //     }
   // )";

   //     // Fragment shader source code
   //     const std::string FragmentShaderSource =
   //         R"(
   //     #version 120
   //     uniform sampler2D texture;
   //     uniform sampler2D texture2;
   //     uniform vec2 texelSize;
   //     uniform vec2 direction;
   //     uniform float alpha;
   //     uniform vec3 color;
   //     uniform int radius;

   //     float gaussian(float x, float sigma) {
   //         float power_2 = x / sigma;
   //         return (1.0 / (sigma * 2.50662827463)) * exp(-0.5 * (power_2 * power_2));
   //     }

   //     void main() {
   //         vec2 texCoord = gl_TexCoord[0].st;

   //         if (direction.y == 1)
   //             if (texture2D(texture2, texCoord).a != 0.0) return;

   //         vec4 blurred_color = vec4(0.0);

   //         for (float r = -radius; r <= radius; r++) {
   //             blurred_color += texture2D(texture, texCoord + r * texelSize * direction) * gaussian(r, radius / 2);
   //         }

   //         if (blurred_color.a > 0) {
   //             if (direction.x == 0) {
   //                 gl_FragColor = vec4(color.rgb / 255.0, blurred_color.a * alpha);
   //             } else {
   //                 gl_FragColor = vec4(color.rgb / 255.0, blurred_color.a);
   //             }
   //         }
   //     }
   // )";

   //     auto ARBShaderObjectsClass = GameClass(Env, "org.lwjgl.opengl.ARBShaderObjects");
   //     auto ARBVertexShaderClass = GameClass(Env, "org.lwjgl.opengl.ARBVertexShader");

   //     auto Class = ARBShaderObjectsClass.GetClass();
   //     auto ShaderClass = ARBVertexShaderClass.GetClass();

   //     jmethodID glCreateProgramObjectARBMethod = Env->GetStaticMethodID(Class, "glCreateProgramObjectARB", "()I");
   //     jmethodID glCreateShaderObjectARBMethod = Env->GetStaticMethodID(Class, "glCreateShaderObjectARB", "(I)I");
   //     jmethodID glShaderSourceARBMethod = Env->GetStaticMethodID(Class, "glShaderSourceARB", "(ILjava/lang/CharSequence;)V");
   //     jmethodID glCompileShaderARBMethod = Env->GetStaticMethodID(Class, "glCompileShaderARB", "(I)V");

   //     this->ProgramId = Env->CallStaticIntMethod(Class, glCreateProgramObjectARBMethod);
   //     // Initialize GLFW
   //    /* if (!glewInit()) {
   //         std::cerr << "Failed to initialize GLFW" << std::endl;
   //         return;
   //     }*/
   //     GLint status;
   //     glGetProgramiv(ProgramId, GL_VALIDATE_STATUS, &status);


   //     std::cout << "statuisssss;;;11" << (status == GL_TRUE) << std::endl;
   //     jstring VertexShaderSourceObject = Env->NewStringUTF(VertexShaderSource.data());
   //     jstring FragmentShaderSourceObject = Env->NewStringUTF(FragmentShaderSource.data());

   //     this->VertexShader = CreateShaderJNI(Env, VertexShaderSourceObject, GL_VERTEX_SHADER_ARB);
   //     this->FragmentShader = CreateShaderJNI(Env, FragmentShaderSourceObject, GL_FRAGMENT_SHADER_ARB);

   //     std::cout << "ProgramId: " << ProgramId << std::endl;
   //     std::cout << "Vertex Shader: " << VertexShader << std::endl;
   //     std::cout << "Fragment Shader: " << FragmentShader << std::endl;

   //     Env->DeleteLocalRef(VertexShaderSourceObject);
   //     Env->DeleteLocalRef(FragmentShaderSourceObject);

   //     OpenGlHelper.GlAttachShader(Env, ProgramId, VertexShader);
   //     OpenGlHelper.GlAttachShader(Env, ProgramId, FragmentShader);
   //     OpenGlHelper.GlLinkProgram(Env, ProgramId);


   //     this->UniformsMap["texture"] = getUniformLocation(Env, ProgramId, "texture");
   //     this->UniformsMap["texture2"] = getUniformLocation(Env, ProgramId, "texture2");
   //     this->UniformsMap["texelSize"] = getUniformLocation(Env, ProgramId, "texelSize");
   //     this->UniformsMap["direction"] = getUniformLocation(Env, ProgramId, "direction");
   //     this->UniformsMap["alpha"] = getUniformLocation(Env, ProgramId, "alpha");
   //     this->UniformsMap["radius"] = getUniformLocation(Env, ProgramId, "radius");
   //     this->UniformsMap["color"] = getUniformLocation(Env, ProgramId, "color");

   //     for (const auto& Uniform : UniformsMap)
   //     {
   //         std::cout << "Uniform " << Uniform.first << " location: " << Uniform.second << std::endl;
   //     }

   //     Setup = true;
   //     Sleep(5000);
   // }
   // //return;
   ///* static auto Minecraft = ::Minecraft(Env);
   // static auto OpenGlHelper = ::OpenGlHelper(Env);

   // const auto World = Minecraft.GetTheWorldPtr(Env);
   // const auto ScaledResolution = std::make_shared<::ScaledResolution>(Env, Minecraft.InstanceObject, true);
   // const auto GlStateManager = ::GlStateManager(Env);
   // const auto RenderManager = Minecraft.GetRenderManagerPtr(Env);
   // const auto EntityRenderer = Minecraft.GetEntityRendererPtr(Env);
   // const auto Timer = Minecraft.GetTimerPtr(Env);

   // glDisable(GL_BLEND);
   // GlStateManager.PushMatrix(Env);
   // GlStateManager.PushAttrb(Env);

   // FramebufferVertical = SetupFramebuffer(Env, FramebufferVertical);
   // FramebufferHorizontal = SetupFramebuffer(Env, FramebufferHorizontal);

   // FramebufferVertical->BindFrameBuffer(Env, true);
   // EntityRenderer->SetupCameraTransformation(Env, Timer->GetRenderPartialTicks(Env), 0);

   // for (auto& entity : GetLivingPlayers(Env)) {
   //     if (IsInView(Env, entity)) {
   //         RenderManager->RenderEntitySimple(Env, entity->InstanceObject, PartialTicks);
   //     }


   // }


   // EntityRenderer->SetupOverlayRendering(Env);

   // glEnable(GL_BLEND);
   // glDisable(GL_ALPHA_TEST);
   // OpenGlHelper.GlBlendFunc(Env, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
   // FramebufferHorizontal->BindFrameBuffer(Env, true);
   // OpenGlHelper.GlUseProgram(Env, ProgramId);

   // UpdateUniforms(Env);
   // glUniform2f(UniformsMap.at("direction"), 1, 0);
   // DrawFrameBuffer(Env, FramebufferVertical, std::make_shared<::ScaledResolution>(Env, Minecraft.GetInstanceObject(), true));

   // OpenGlHelper.SetActiveTexture(Env, OpenGlHelper.LightmapTexUnit(Env) + 7);
   // DrawFrameBuffer(Env, FramebufferVertical, std::make_shared<::ScaledResolution>(Env, Minecraft.GetInstanceObject(), true));
   // OpenGlHelper.SetActiveTexture(Env, OpenGlHelper.DefaultTexUnit(Env));

   // Minecraft.GetFrameBufferPtr(Env)->BindFrameBuffer(Env, true);
   // glUniform2f(UniformsMap.at("direction"), 0, 1);
   // DrawFrameBuffer(Env, FramebufferHorizontal, std::make_shared<::ScaledResolution>(Env, Minecraft.GetInstanceObject(), true));
   // OpenGlHelper.GlUseProgram(Env, 0);
   // GlStateManager.PopMatrix(Env);
   // GlStateManager.PopAttrib(Env);
   // glEnable(GL_ALPHA_TEST);*/
   // // Your existing code with added error checking
   // static auto Minecraft = ::Minecraft(Env);
   // static auto OpenGlHelper = ::OpenGlHelper(Env);

   // const auto World = Minecraft.GetTheWorldPtr(Env);
   // const auto ScaledResolution = std::make_shared<::ScaledResolution>(Env, Minecraft.InstanceObject, true);
   // const auto GlStateManager = ::GlStateManager(Env);
   // const auto RenderManager = Minecraft.GetRenderManagerPtr(Env);
   // const auto EntityRenderer = Minecraft.GetEntityRendererPtr(Env);
   // const auto Timer = Minecraft.GetTimerPtr(Env);

   // // Disable blend and push matrix/attributes
   // glDisable(GL_BLEND);
   // checkGLError("glDisable(GL_BLEND)");
   // GlStateManager.PushMatrix(Env);
   // checkGLError("GlStateManager.PushMatrix");
   // GlStateManager.PushAttrb(Env);
   // checkGLError("GlStateManager.PushAttrb");

   // FramebufferVertical = SetupFramebuffer(Env, FramebufferVertical);
   // FramebufferHorizontal = SetupFramebuffer(Env, FramebufferHorizontal);

   // FramebufferVertical->BindFrameBuffer(Env, true);
   // checkGLError("FramebufferVertical->BindFrameBuffer");

   // EntityRenderer->SetupCameraTransformation(Env, Timer->GetRenderPartialTicks(Env), 0);

   // for (auto& entity : GetLivingPlayers(Env)) {
   //     if (IsInView(Env, entity)) {
   //         RenderManager->RenderEntitySimple(Env, entity->InstanceObject, PartialTicks);
   //     }
   // }

   // EntityRenderer->SetupOverlayRendering(Env);

   // // Enable blend and disable alpha test
   // glEnable(GL_BLEND);
   // checkGLError("glEnable(GL_BLEND)");
   // glDisable(GL_ALPHA_TEST);
   // checkGLError("glDisable(GL_ALPHA_TEST)");

   // OpenGlHelper.GlBlendFunc(Env, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
   // checkGLError("OpenGlHelper.GlBlendFunc");

   // FramebufferHorizontal->BindFrameBuffer(Env, true);
   // checkGLError("FramebufferHorizontal->BindFrameBuffer");

   // OpenGlHelper.GlUseProgram(Env, ProgramId);
   // checkGLError("OpenGlHelper.GlUseProgram");

   // UpdateUniforms(Env);
   // checkGLError("UpdateUniforms");

   // setUniform2f(Env, UniformsMap.at("direction"), 1, 0);
   // checkGLError("glUniform2f");

   // DrawFrameBuffer(Env, FramebufferVertical, std::make_shared<::ScaledResolution>(Env, Minecraft.GetInstanceObject(), true));

   // // Set active texture and draw framebuffer
   // OpenGlHelper.SetActiveTexture(Env, OpenGlHelper.LightmapTexUnit(Env) + 7);
   // checkGLError("OpenGlHelper.SetActiveTexture");
   // DrawFrameBuffer(Env, FramebufferVertical, std::make_shared<::ScaledResolution>(Env, Minecraft.GetInstanceObject(), true));

   // OpenGlHelper.SetActiveTexture(Env, OpenGlHelper.DefaultTexUnit(Env));
   // checkGLError("OpenGlHelper.SetActiveTexture");

   // Minecraft.GetFrameBufferPtr(Env)->BindFrameBuffer(Env, true);
   // checkGLError("Minecraft.GetFrameBufferPtr->BindFrameBuffer");

   // setUniform2f(Env, UniformsMap.at("direction"), 0, 1);
   // checkGLError("glUniform2f");

   // DrawFrameBuffer(Env, FramebufferHorizontal, std::make_shared<::ScaledResolution>(Env, Minecraft.GetInstanceObject(), true));

   // OpenGlHelper.GlUseProgram(Env, 0);
   // checkGLError("OpenGlHelper.GlUseProgram");

   // GlStateManager.PopMatrix(Env);
   // checkGLError("GlStateManager.PopMatrix");
   // GlStateManager.PopAttrib(Env);
   // checkGLError("GlStateManager.PopAttrib");

   // // Re-enable alpha test
   // glEnable(GL_ALPHA_TEST);
   // checkGLError("glEnable(GL_ALPHA_TEST)");
}
#include "src/sdk/game/minecraft/modelbiped.hpp"
void GlowRenderer::SetRotationAnglesDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
   /* if (!InstancePtr->GetJniEnv()) {
        return;
    }
    const double PI = 3.14159265358979323846;

    JNIEnv* Env = Thread->GetEnv();
    EnvRegister::RegisterThreadEnv(Env);

    const auto this_t = std::make_shared<ModelBiped>(Env,HookManager->GetObjectParam(Frame, 0));

    const auto limbSwing = HookManager->get_primitive_param_at<jfloat>(Frame, 1);
    const auto limbSwingAmount = HookManager->get_primitive_param_at<jfloat>(Frame, 2);
    const auto ageInTicks = HookManager->get_primitive_param_at<jfloat>(Frame, 3);
    const auto netHeadYaw = HookManager->get_primitive_param_at<jfloat>(Frame, 4);
    const auto headPitch = HookManager->get_primitive_param_at<jfloat>(Frame, 5);
    const auto scaleFactor = HookManager->get_primitive_param_at<jfloat>(Frame, 6);
    auto entityIn = std::make_shared<Player>(Env, HookManager->GetObjectParam(Frame, 7));

    const auto Model = this_t->GetModel(Env);
    const auto& BipedHead = Model.BipedHead;
    const auto& BipedRightArm = Model.BipedRightArm;
    const auto& BipedLeftArm = Model.BipedLeftArm;
    const auto& BipedRightLeg = Model.BipedRightLeg;
    const auto& BipedLeftLeg = Model.BipedLeftLeg;

    BipedHead->SetRotationAngle(Env, { headPitch / (180.F / (float)PI), netHeadYaw / (180.F / (float)PI), BipedHead->RotationAngle(Env).Z });
    BipedRightArm->SetRotationAngle(Env, { std::cos(limbSwing * 0.6662F + (float)PI) * 2.0F * limbSwingAmount * 0.5F, BipedRightArm->RotationAngle(Env).Y, 0.0F });
    BipedLeftArm->SetRotationAngle(Env, { std::cos(limbSwing * 0.6662F) * 2.0F * limbSwingAmount * 0.5F, BipedLeftArm->RotationAngle(Env).Y, 0.0F });
    BipedRightLeg->SetRotationAngle(Env, { cos(limbSwing * 0.6662F) * 1.4F * limbSwingAmount, 0.0F, BipedRightLeg->RotationAngle(Env).Z });
    BipedLeftLeg->SetRotationAngle(Env, { cos(limbSwing * 0.6662F) * 1.4F * limbSwingAmount, 0.0F, BipedLeftLeg->RotationAngle(Env).Z });

    SkeletonMap[entityIn] = Skeleton
    {
        BipedHead->RotationAngle(Env),
        BipedRightArm->RotationAngle(Env),
        BipedLeftArm->RotationAngle(Env),
        BipedRightLeg->RotationAngle(Env),
        BipedLeftLeg->RotationAngle(Env),
    };*/

    /*if (this.isRiding)
    {
        this.bipedRightArm.rotateAngleX += -((float)Math.PI / 5F);
        this.bipedLeftArm.rotateAngleX += -((float)Math.PI / 5F);
        this.bipedRightLeg.rotateAngleX = -((float)Math.PI * 2F / 5F);
        this.bipedLeftLeg.rotateAngleX = -((float)Math.PI * 2F / 5F);
        this.bipedRightLeg.rotateAngleY = ((float)Math.PI / 10F);
        this.bipedLeftLeg.rotateAngleY = -((float)Math.PI / 10F);
    }

    if (this.heldItemLeft != 0)
    {
        this.bipedLeftArm.rotateAngleX = this.bipedLeftArm.rotateAngleX * 0.5F - ((float)Math.PI / 10F) * (float)this.heldItemLeft;
    }

    this.bipedRightArm.rotateAngleY = 0.0F;
    this.bipedRightArm.rotateAngleZ = 0.0F;

    switch (this.heldItemRight)
    {
    case 0:
    case 2:
    default:
        break;

    case 1:
        this.bipedRightArm.rotateAngleX = this.bipedRightArm.rotateAngleX * 0.5F - ((float)Math.PI / 10F) * (float)this.heldItemRight;
        break;

    case 3:
        this.bipedRightArm.rotateAngleX = this.bipedRightArm.rotateAngleX * 0.5F - ((float)Math.PI / 10F) * (float)this.heldItemRight;
        this.bipedRightArm.rotateAngleY = -0.5235988F;
    }

    this.bipedLeftArm.rotateAngleY = 0.0F;

    if (this.swingProgress > -9990.0F)
    {
        float f = this.swingProgress;
        this.bipedBody.rotateAngleY = MathHelper.sin(MathHelper.sqrt_float(f) * (float)Math.PI * 2.0F) * 0.2F;
        this.bipedRightArm.rotationPointZ = MathHelper.sin(this.bipedBody.rotateAngleY) * 5.0F;
        this.bipedRightArm.rotationPointX = -MathHelper.cos(this.bipedBody.rotateAngleY) * 5.0F;
        this.bipedLeftArm.rotationPointZ = -MathHelper.sin(this.bipedBody.rotateAngleY) * 5.0F;
        this.bipedLeftArm.rotationPointX = MathHelper.cos(this.bipedBody.rotateAngleY) * 5.0F;
        this.bipedRightArm.rotateAngleY += this.bipedBody.rotateAngleY;
        this.bipedLeftArm.rotateAngleY += this.bipedBody.rotateAngleY;
        this.bipedLeftArm.rotateAngleX += this.bipedBody.rotateAngleY;
        f = 1.0F - this.swingProgress;
        f = f * f;
        f = f * f;
        f = 1.0F - f;
        float f1 = MathHelper.sin(f * (float)Math.PI);
        float f2 = MathHelper.sin(this.swingProgress * (float)Math.PI) * -(this.bipedHead.rotateAngleX - 0.7F) * 0.75F;
        this.bipedRightArm.rotateAngleX = (float)((double)this.bipedRightArm.rotateAngleX - ((double)f1 * 1.2D + (double)f2));
        this.bipedRightArm.rotateAngleY += this.bipedBody.rotateAngleY * 2.0F;
        this.bipedRightArm.rotateAngleZ += MathHelper.sin(this.swingProgress * (float)Math.PI) * -0.4F;
    }

    if (this.isSneak)
    {
        this.bipedBody.rotateAngleX = 0.5F;
        this.bipedRightArm.rotateAngleX += 0.4F;
        this.bipedLeftArm.rotateAngleX += 0.4F;
        this.bipedRightLeg.rotationPointZ = 4.0F;
        this.bipedLeftLeg.rotationPointZ = 4.0F;
        this.bipedRightLeg.rotationPointY = 9.0F;
        this.bipedLeftLeg.rotationPointY = 9.0F;
        this.bipedHead.rotationPointY = 1.0F;
    }
    else
    {
        this.bipedBody.rotateAngleX = 0.0F;
        this.bipedRightLeg.rotationPointZ = 0.1F;
        this.bipedLeftLeg.rotationPointZ = 0.1F;
        this.bipedRightLeg.rotationPointY = 12.0F;
        this.bipedLeftLeg.rotationPointY = 12.0F;
        this.bipedHead.rotationPointY = 0.0F;
    }

    this.bipedRightArm.rotateAngleZ += MathHelper.cos(ageInTicks * 0.09F) * 0.05F + 0.05F;
    this.bipedLeftArm.rotateAngleZ -= MathHelper.cos(ageInTicks * 0.09F) * 0.05F + 0.05F;
    this.bipedRightArm.rotateAngleX += MathHelper.sin(ageInTicks * 0.067F) * 0.05F;
    this.bipedLeftArm.rotateAngleX -= MathHelper.sin(ageInTicks * 0.067F) * 0.05F;

    if (this.aimedBow)
    {
        float f3 = 0.0F;
        float f4 = 0.0F;
        this.bipedRightArm.rotateAngleZ = 0.0F;
        this.bipedLeftArm.rotateAngleZ = 0.0F;
        this.bipedRightArm.rotateAngleY = -(0.1F - f3 * 0.6F) + this.bipedHead.rotateAngleY;
        this.bipedLeftArm.rotateAngleY = 0.1F - f3 * 0.6F + this.bipedHead.rotateAngleY + 0.4F;
        this.bipedRightArm.rotateAngleX = -((float)Math.PI / 2F) + this.bipedHead.rotateAngleX;
        this.bipedLeftArm.rotateAngleX = -((float)Math.PI / 2F) + this.bipedHead.rotateAngleX;
        this.bipedRightArm.rotateAngleX -= f3 * 1.2F - f4 * 0.4F;
        this.bipedLeftArm.rotateAngleX -= f3 * 1.2F - f4 * 0.4F;
        this.bipedRightArm.rotateAngleZ += MathHelper.cos(ageInTicks * 0.09F) * 0.05F + 0.05F;
        this.bipedLeftArm.rotateAngleZ -= MathHelper.cos(ageInTicks * 0.09F) * 0.05F + 0.05F;
        this.bipedRightArm.rotateAngleX += MathHelper.sin(ageInTicks * 0.067F) * 0.05F;
        this.bipedLeftArm.rotateAngleX -= MathHelper.sin(ageInTicks * 0.067F) * 0.05F;
    }*/

}

std::shared_ptr<GlowRenderer> GlowRendererPtr = std::make_shared<GlowRenderer>();