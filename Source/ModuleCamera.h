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
	void SetAspectRatio(unsigned int screen_width, unsigned int screen_height);
	void SetHorizontalFov(float fov_deg);

	void RefreshFov();
	void WindowResized(unsigned int screen_width, unsigned int screen_height);

	// SetPlaneDistances()
	// Position()
	// Orientation()
	// LookAt()
	// GetProjectionMatrix() correct orientation
	// GetViewMatrix() correct orientation
	// Detect resize and update fov
	
	// Dont do orbit
	// Add debugdraw to use
	

private:
	Frustum frustum;
	float aspect_ratio;
	float horizontal_fov_deg;
	float horizontal_fov;
	float3 look_position;
	float3 camera_position;
	float4x4 model;
	float4x4 view;	
	float4x4 projection;
};

