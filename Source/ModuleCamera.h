#pragma once
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h"


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;
	
	void SetPosition(const float3& position);
	void Rotate(float pitch, float yaw);
	void SetAspectRatio(unsigned int screen_width, unsigned int screen_height);
	void SetHorizontalFov(float fov_deg);
	void Zoom(float deg_diff);
	void LookAt(const float3& position);
	void CameraController();
	void SetPlaneDistances(const float near_distance, const float far_distance);
	void WindowResized(unsigned int screen_width, unsigned int screen_height);

	void OptionsMenu();

	// TODO: Wasd + mouse on right click
	// TODO: Focus with keyboard to model (f)
	// TODO Orbit object (alt + left click)

	

	float4x4 GetView() const;
	float4x4 GetProjection() const;
	
	// Orbit and more controls

	

	

private:
	Frustum frustum;
	float aspect_ratio;
	float horizontal_fov;
	float3 look_position;	

	bool locked = false;
	float3 position;
	float near_distance;
	float far_distance;
};

