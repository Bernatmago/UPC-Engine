#include "glew.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleShader.h"
#include "ModuleWindow.h"

#include "Math/MathConstants.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}



bool ModuleCamera::Init()
{
    // TODO: Remove redundant variables when it works
    model = float4x4::identity;
    look_position = float3(0.0f, 0.0f, 0.0f);
    camera_position = float3(1.0f, 1.0f, 0.5f);

    frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
    auto screen_surface = App->window->screen_surface;
    SetAspectRatio(screen_surface->w, screen_surface->h);
    SetHorizontalFov(90.0f);
    RefreshFov();
    frustum.SetViewPlaneDistances(0.1f, 100.0f);

    frustum.SetPos(camera_position);
    frustum.SetFront(float3::unitZ);
    frustum.SetUp(float3::unitY);    

    float3 direction = look_position - frustum.Pos();
    // localForward, targetDirection, localUp, worldUp
    float3x3 look_rotation = float3x3::LookAt(frustum.Front(), direction.Normalized(), frustum.Up(), float3::unitY);
    frustum.SetFront(look_rotation.MulDir(frustum.Front()).Normalized());
    frustum.SetUp(look_rotation.MulDir(frustum.Up()).Normalized());


    
    return true;
}

update_status ModuleCamera::PreUpdate()
{
    // TODO: Manage uniforms properly and check if it is better to transpose in opengl
    projection = frustum.ProjectionMatrix().Transposed();
    view = float4x4(frustum.ViewMatrix()).Transposed();

    auto shader_id = App->shader->shader_id;
    glUseProgram(shader_id);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "proj"), 1, GL_FALSE, &projection[0][0]);
    return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
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

void ModuleCamera::SetPosition(const float3& position)
{
    camera_position = position;
    frustum.SetPos(camera_position);
}

void ModuleCamera::SetAspectRatio(unsigned int screen_width, unsigned int screen_height)
{
    aspect_ratio = (float)screen_width / (float)screen_height;
}

void ModuleCamera::SetHorizontalFov(float fov_deg)
{
    static const float deg_to_rad = pi / 180.0f;
    horizontal_fov_deg = fov_deg;
    horizontal_fov = fov_deg * deg_to_rad;
}

void ModuleCamera::RefreshFov()
{
    frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, aspect_ratio);
}

void ModuleCamera::WindowResized(unsigned int screen_width, unsigned int screen_height)
{
    SetAspectRatio(screen_width, screen_height);
    RefreshFov();
}
