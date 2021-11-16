#include "glew.h"

#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleShader.h"
#include "Math/MathConstants.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}





bool ModuleCamera::Init()
{
    // TODO Not hardoded aspect ratio
    const float aspect_ratio = 16.0f / 9.0f;
    const float horizontal_fov_deg = 90.0f;
    const float deg_to_rad = pi / 180.0f;

    float4x4 model = float4x4::identity;
    const float3 look_position = float3(0.0f, 0.0f, 0.0f);
    const float3 camera_position = float3(1.0f, 1.0f, 2.0f);


    frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
    frustum.SetViewPlaneDistances(0.1f, 100.0f);

    frustum.SetHorizontalFovAndAspectRatio(deg_to_rad * horizontal_fov_deg, aspect_ratio );
    frustum.SetPos(camera_position);
    frustum.SetFront(float3::unitZ);
    frustum.SetUp(float3::unitY);

    projection = frustum.ProjectionMatrix().Transposed();

    float3 direction = look_position - frustum.Pos();

    // localForward, targetDirection, localUp, worldUp
    float3x3 look_rotation = float3x3::LookAt(frustum.Front(), direction.Normalized(), frustum.Up(), float3::unitY);
    frustum.SetFront(look_rotation.MulDir(frustum.Front()).Normalized());
    frustum.SetUp(look_rotation.MulDir(frustum.Up()).Normalized());

    view = float4x4(frustum.ViewMatrix()).Transposed();

    // TODO: Manage uniforms properly and check if it is better to transpose in opengl
    auto shader_id = App->shader->shader_id;

    glUseProgram(shader_id);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader_id, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader_id, "proj"), 1, GL_FALSE, &projection[0][0]);

    return true;
}

update_status ModuleCamera::PreUpdate()
{
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
