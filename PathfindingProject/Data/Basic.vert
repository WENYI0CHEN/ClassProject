// glsl
attribute vec2 a_position;
attribute vec4 a_color; // input
attribute vec2 a_uv;

varying vec4 v_color; // output
varying vec2 v_uv;

uniform vec2 u_scale;
uniform float u_angle;
uniform vec2 u_offset;
uniform vec2 u_cameraPosition;
uniform vec2 u_projectionScale;

vec2 calculateWorldSpacePosition(vec2 objectSpacePosition);

void main()
{
	vec2 objectSpacePosition = a_position;
	vec2 worldSpacePosition = calculateWorldSpacePosition(objectSpacePosition);
	vec2 viewSpacePosition = worldSpacePosition - u_cameraPosition;
	vec2 clipSpacePosition = viewSpacePosition * u_projectionScale;

	gl_Position = vec4( clipSpacePosition, 0.0, 1.0 );

	v_color = a_color;
	v_uv = a_uv;
}

vec2 calculateWorldSpacePosition(vec2 objectSpacePosition)
{
	vec2 pos = objectSpacePosition;

	pos *= u_scale;
	pos = vec2(
		pos.x*cos(u_angle) - pos.y*sin(u_angle),
		pos.x*sin(u_angle) + pos.y*cos(u_angle)
	);
	pos += u_offset;

	return pos;
}
