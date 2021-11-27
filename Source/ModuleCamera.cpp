#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include "glew.h"
#include "assimp/scene.h"
#include "Math/MathConstants.h"

static const float deg_to_rad = pi / 180.0f;

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
    frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
    auto screen_surface = App->window->screen_surface;
    SetAspectRatio(screen_surface->w, screen_surface->h);
    SetHorizontalFov(90.0f);
    SetPlaneDistances(0.1f, 100.0f);
    SetPosition(float3(0.0f, 0.5f, 10.0f));
    float3x3 rotation = float3x3::identity;
    frustum.SetFront(rotation.WorldZ());
    frustum.SetUp(rotation.WorldY());
    LookAt(float3(0.0f, 0.0f, 0.0f));
    return true;
}

update_status ModuleCamera::Update(const float delta)
{
    
    CameraController(delta);
    SetPlaneDistances(near_distance, far_distance);
    if (locked)
        LookAt(App->renderer->model->GetPosition());

    return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
    return true;
}

void ModuleCamera::SetPosition(const float3& new_position)
{
    frustum.SetPos(position = new_position);
}

void ModuleCamera::SetAspectRatio(unsigned int screen_width, unsigned int screen_height)
{
    aspect_ratio = (float)screen_width / (float)screen_height;
    frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, aspect_ratio);
}

void ModuleCamera::SetHorizontalFov(float fov_deg)
{
    horizontal_fov = fov_deg * deg_to_rad;
    frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, aspect_ratio);
}

void ModuleCamera::Zoom(float deg_diff)
{
    static const float max_fov = 2.5f;
    static const float min_fov = 1.0f;
    horizontal_fov += deg_diff * deg_to_rad;
    if (horizontal_fov > max_fov)
        horizontal_fov = max_fov;
    if (horizontal_fov < min_fov)
        horizontal_fov = min_fov;
    
    LOG("HF %f", horizontal_fov)
    frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, aspect_ratio);
}

void ModuleCamera::LookAt(const float3& look_position)
{
    float3 direction = look_position - frustum.Pos();
    // localForward, targetDirection, localUp, worldUp
    float3x3 look_dir = float3x3::LookAt(frustum.Front(), direction.Normalized(), frustum.Up(), float3::unitY);
    frustum.SetFront(look_dir.MulDir(frustum.Front()).Normalized());
    frustum.SetUp(look_dir.MulDir(frustum.Up()).Normalized());
}

void ModuleCamera::Rotate(float pitch, float yaw)
{
    if (yaw != 0.0f) {
        // Rotate in Y absolut axis
        Quat rot = Quat::RotateY(yaw);
        frustum.SetFront(rot.Mul(frustum.Front()).Normalized());
        frustum.SetUp(rot.Mul(frustum.Up()).Normalized());
    }

    if (pitch != 0.0f) {
        // Rotate in X local axis
        Quat rot = Quat::RotateAxisAngle(frustum.WorldRight(), pitch);
        float3 a = rot.Mul(frustum.Up()).Normalized();
        frustum.SetUp(a);      
        frustum.SetFront(rot.Mul(frustum.Front()).Normalized());
    }
}

void ModuleCamera::CameraController(const float delta)
{
   static const float move_speed = 15.0f;
   static const float speed_modifier = 2.0f;
   static const float rot_speed = 2.5f;
   static const float zoom_speed = 3.0f;
      
   if (App->input->GetMouseButton(SDL_BUTTON_RIGHT)) {   
       int moved_x, moved_y;
       App->input->GetMouseDelta(moved_x, moved_y);

       if (moved_x != 0 or moved_y != 0)
           Rotate(-rot_speed * (float)moved_y * delta, -rot_speed * (float)moved_x * delta);

       float effective_speed = move_speed;
       if (App->input->GetKeyMod(KMOD_SHIFT))
           effective_speed *= speed_modifier;

       if (App->input->GetKey(SDL_SCANCODE_W))
           position += frustum.Front() * effective_speed * delta;
       if (App->input->GetKey(SDL_SCANCODE_S))
           position -= frustum.Front() * effective_speed * delta;
       if (App->input->GetKey(SDL_SCANCODE_A))
           position -= frustum.WorldRight() * effective_speed * delta;
       if (App->input->GetKey(SDL_SCANCODE_D))
           position += frustum.WorldRight() * effective_speed * delta;
       if (App->input->GetKey(SDL_SCANCODE_Q))
           position += frustum.Up() * effective_speed * delta;
       if (App->input->GetKey(SDL_SCANCODE_E))
           position -= frustum.Up() * effective_speed * delta;
   }
   else if (App->input->GetKey(SDL_SCANCODE_F)) {
       LookAt(App->renderer->model->GetPosition());
   }

   int scrolled_y = App->input->GetScrollDelta();
   if (scrolled_y != 0) {
       Zoom(zoom_speed * -scrolled_y);
   }

   // Extra rotation keys
   if (App->input->GetKey(SDL_SCANCODE_UP))
       Rotate(rot_speed * delta, 0.0f);
   if (App->input->GetKey(SDL_SCANCODE_DOWN))
       Rotate(-rot_speed * delta, 0.0f);
   if (App->input->GetKey(SDL_SCANCODE_LEFT))
       Rotate(0.0f, rot_speed * delta);
   if (App->input->GetKey(SDL_SCANCODE_RIGHT))
       Rotate(0.0f, -rot_speed * delta);      
   
   frustum.SetPos(position);
}

void ModuleCamera::SetPlaneDistances(const float near_dist, const float far_dist)
{
    frustum.SetViewPlaneDistances(near_distance = near_dist, far_distance = far_dist);
}

void ModuleCamera::WindowResized(unsigned int screen_width, unsigned int screen_height)
{
    SetAspectRatio(screen_width, screen_height);
}

float4x4 ModuleCamera::GetView() const
{
    return float4x4(frustum.ViewMatrix());
}

float4x4 ModuleCamera::GetProjection() const
{
    return frustum.ProjectionMatrix();
}

void ModuleCamera::OptionsMenu()
{
    ImGui::SliderFloat2("cX, cY", &position[0], -5.0f, 5.0f);
    ImGui::SliderFloat("cZ", &position.z, 0.5f, 10.0f);
    ImGui::SliderFloat("N", &near_distance, 0.1f, 500.0f);
    ImGui::SliderFloat("F", &far_distance, 0.1f, 500.0f);
    ImGui::Checkbox("Look Center", &locked);
    if (locked) {
        ImGui::SameLine();
        ImGui::Text("Rotation disabled");
    }
}