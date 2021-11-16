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
	float aspect_ratio;
	float horizontal_fov_deg;
	float horizontal_fov;
	float4x4 view;
	Frustum frustum;
	float4x4 projection;
};

