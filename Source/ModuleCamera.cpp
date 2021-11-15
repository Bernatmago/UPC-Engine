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


    frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
    frustum.SetViewPlaneDistances(0.1f, 200.0f);

    frustum.SetHorizontalFovAndAspectRatio(deg_to_rad * aspect_ratio, aspect_ratio );
    frustum.SetPos(float3(0.0f, 1.0f, -2.0f));
    frustum.SetFront(float3::unitZ);
    frustum.SetUp(float3::unitY);

    // TODO: Lookat
    projection = frustum.ProjectionMatrix().Transposed();

    LookAt(float3(0.0f, 0.0f, 0.0f));    

    view = float4x4(frustum.ViewMatrix()).Transposed();

    auto model = float4x4::zero;
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

void ModuleCamera::LookAt(const float3& position)
{
    float3 dir = position - frustum.Pos();

    // TODO: Understand better
    float3x3 a = float3x3::LookAt(frustum.Front(), dir.Normalized(), frustum.Up(), float3::unitY);
    frustum.SetFront(a.MulDir(frustum.Front().Normalized()));
    frustum.SetUp(a.MulDir(frustum.Up().Normalized()));
}
