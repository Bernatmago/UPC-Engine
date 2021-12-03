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
	update_status Update(const float delta) override;
	bool CleanUp() override;
	
	void SetPosition(const float3& position);
	void Rotate(float pitch, float yaw);
	void SetAspectRatio(unsigned int screen_width, unsigned int screen_height);
	void SetHorizontalFov(float fov_deg);
	void Zoom(float deg_diff);
	void LookAt(const float3& position);
	void CameraController(const float delta);
	void UpdatePlaneDistances();
	void SetPlaneDistances(const float near_distance, const float far_distance);
	void WindowResized(unsigned int screen_width, unsigned int screen_height);

	void OptionsMenu();

	// TODO Orbit object (alt + left click)
	float4x4 GetView() const;
	float4x4 GetProjection() const;
	
	// Orbit and more controls
	

private:
	struct RenderPlanes {
		float near_plane;
		float far_plane;
	};

	RenderPlanes planes;

	Frustum frustum;
	float aspect_ratio;
	float horizontal_fov;
	float3 look_position;	

	bool locked = false;
	float3 position;
};

