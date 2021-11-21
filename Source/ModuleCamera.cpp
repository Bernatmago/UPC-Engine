#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

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
    // TODO: Remove redundant variables when it works
    frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
    auto screen_surface = App->window->screen_surface;
    SetAspectRatio(screen_surface->w, screen_surface->h);
    SetHorizontalFov(90.0f);
    SetPlaneDistances(0.1f, 100.0f);
    SetPosition(float3(0.0f, 0.5f, 10.0f));
    rotation = float3x3::identity;
    frustum.SetFront(rotation.WorldZ());
    frustum.SetUp(rotation.WorldY());
    LookAt(float3(0.0f, 0.0f, 0.0f));
    return true;
}

update_status ModuleCamera::PreUpdate()
{    
    return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
    
    CameraController();
    SetPlaneDistances(near_distance, far_distance);

    if (locked)
        LookAt(float3(0.0f, 0.0f, 0.0f));

    return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
    return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
    return false;
}

void ModuleCamera::SetPosition(const float3& new_position)
{
    frustum.SetPos(position = new_position);
}

void ModuleCamera::Rotate(float pitch, float yaw)
{
    // TODO: FIX..
    float3x3 current = float3x3::RotateAxisAngle(rotation.WorldX(), pitch * deg_to_rad) *
        float3x3::RotateAxisAngle(float3::unitY, -yaw * deg_to_rad);
    rotation = current.Mul(rotation);
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

void ModuleCamera::LookAt(const float3& look_position)
{
    float3 direction = look_position - frustum.Pos();
    // localForward, targetDirection, localUp, worldUp
    // TODO: We are using rotation in two different ways, fix
    rotation = float3x3::LookAt(frustum.Front(), direction.Normalized(), frustum.Up(), float3::unitY);
    frustum.SetFront(rotation.MulDir(frustum.Front()).Normalized());
    frustum.SetUp(rotation.MulDir(frustum.Up()).Normalized());
}

void ModuleCamera::SetPlaneDistances(const float near_dist, const float far_dist)
{
    frustum.SetViewPlaneDistances(near_distance = near_dist, far_distance = far_dist);
}

void ModuleCamera::WindowResized(unsigned int screen_width, unsigned int screen_height)
{
    SetAspectRatio(screen_width, screen_height);
}

void ModuleCamera::CameraController()
{
   static const float speed = 0.02f;
   uint32_t delta = App->GetDeltaTime();   
    
   if (!locked && App->input->GetMouseButton(SDL_BUTTON_RIGHT)) {
       int delta_x, delta_y;
       App->input->GetMouseDelta(delta_x, delta_y);
       //Rotate(speed * delta_y * delta, speed * delta_x * delta);
       //Rotate(speed * delta_y * delta, speed * delta_x * delta);
       Rotate(speed * delta_y * delta, 0.0f);
       Rotate(0.0f, speed * delta_x * delta);
       //LOG("%d, %d", delta_x, delta_y);
   }
   if (App->input->GetKey(SDL_SCANCODE_UP))
       Rotate(-speed * 5 * delta, 0.0f);
   if (App->input->GetKey(SDL_SCANCODE_DOWN))
       Rotate(speed * 5 * delta, 0.0f);
   if (App->input->GetKey(SDL_SCANCODE_LEFT))
       Rotate(0.0f, -speed * 5 * delta);
   if (App->input->GetKey(SDL_SCANCODE_RIGHT))
       Rotate(0.0f, speed * 5 * delta);


   // TODO: Move along frustrum front
   if (App->input->GetKey(SDL_SCANCODE_W))
       position += rotation.WorldZ() * speed * delta;
   if (App->input->GetKey(SDL_SCANCODE_S))
       position -= rotation.WorldZ()* speed * delta;
   if (App->input->GetKey(SDL_SCANCODE_A))
       position += rotation.WorldX() * speed * delta;
   if (App->input->GetKey(SDL_SCANCODE_D))
       position -= rotation.WorldX() * speed * delta;
   if (App->input->GetKey(SDL_SCANCODE_Q))
       position += rotation.WorldY() * speed * delta;
   if (App->input->GetKey(SDL_SCANCODE_E))
       position -= rotation.WorldY() * speed * delta;
   
   frustum.SetPos(position);
   frustum.SetFront(rotation.WorldZ());
   frustum.SetUp(rotation.WorldY());
   //frustum.set
}

float4x4 ModuleCamera::GetGLView() const
{
    return float4x4(frustum.ViewMatrix()).Transposed();
}

float4x4 ModuleCamera::GetView() const
{
    return float4x4(frustum.ViewMatrix());
}

float4x4 ModuleCamera::GetGLProjection() const
{
    return frustum.ProjectionMatrix().Transposed();
}

float4x4 ModuleCamera::GetProjection() const
{
    return frustum.ProjectionMatrix();
}