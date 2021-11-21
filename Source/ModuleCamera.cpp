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
    SetPosition(float3(0.0f, 0.0f, 10.0f));
    
    frustum.SetFront(float3::unitZ);
    frustum.SetUp(float3::unitY); 
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

    SetPosition(position);
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
    float3x3 look_rotation = float3x3::LookAt(frustum.Front(), direction.Normalized(), frustum.Up(), float3::unitY);
    frustum.SetFront(look_rotation.MulDir(frustum.Front()).Normalized());
    frustum.SetUp(look_rotation.MulDir(frustum.Up()).Normalized());
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
   if (App->input->GetMouseButton(SDL_BUTTON_RIGHT)) {
        int delta_x, delta_y;
        App->input->GetMouseDelta(delta_x, delta_y);
        /*float3x3 rotation = float3x3::RotateAxisAngle(float3::unitX, delta_y * delta * deg_to_rad) *
            rotation = float3x3::RotateAxisAngle(float3::unitY, -delta_x * delta * deg_to_rad);
        */
        LOG("%d, %d", delta_x, delta_y);
    }   

    
    //App->input->GetMouseMotion(deltaX, deltaY);
    if (App->input->GetKey(SDL_SCANCODE_W))
        position.z -= speed * delta;
    if (App->input->GetKey(SDL_SCANCODE_S))
        position.z += speed * delta;
    if (App->input->GetKey(SDL_SCANCODE_A))
        position.x -= speed * delta;
    if (App->input->GetKey(SDL_SCANCODE_D))
        position.x += speed * delta;
    if (App->input->GetKey(SDL_SCANCODE_Q))
        position.y += speed * delta;
    if (App->input->GetKey(SDL_SCANCODE_E))
        position.y -= speed * delta;
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