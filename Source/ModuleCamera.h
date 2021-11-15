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

	void LookAt(const float3& position);

	// SetFOV (horizontal fov)
	// SetAspectRatio (vertical fov)
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
	float4x4 view;
	Frustum frustum;
	float4x4 projection;
};

