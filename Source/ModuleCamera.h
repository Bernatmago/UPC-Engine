#pragma once
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h"


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	
	void SetPosition(const float3& position);
	void Rotate(float pitch, float yaw);
	void SetAspectRatio(unsigned int screen_width, unsigned int screen_height);
	void SetHorizontalFov(float fov_deg);
	void LookAt(const float3& position);
	void SetPlaneDistances(const float near_distance, const float far_distance);

	void WindowResized(unsigned int screen_width, unsigned int screen_height);

	void CameraController();

	float4x4 GetGLView() const;
	float4x4 GetView() const;
	float4x4 GetGLProjection() const;
	float4x4 GetProjection() const;
	
	// Orientation()
	
	// Dont do orbit
	// Add debugdraw to use

	bool locked = false;
	float3 position;
	//float3x3 rotation;
	float near_distance;
	float far_distance;

	

private:
	Frustum frustum;
	float aspect_ratio;
	float horizontal_fov;
	float3 look_position;	
};

