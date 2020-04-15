#version 120

attribute vec4 vPosition;
attribute vec4 vNormal;
varying vec4 color;

uniform mat4 ctm;
uniform mat4 modelview;
uniform mat4 projection;
uniform int isShadow;
uniform vec4 light_s;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
vec4 ambient, diffuse, specular;
uniform float AttenuationConstant, AttenuationLinear, AttenuationQuadratic, shininess;
uniform int islight;
uniform vec4 light_change;

void main()
{
	if(isShadow == 1)
	{
		vec4 v_temp = ctm*vPosition;
		vec4 light_temp = (light_s) + light_change;
		float x = light_temp.x - (light_temp.y*((light_temp.x - v_temp.x) / (light_temp.y - v_temp.y)));
		float y = 0.001;
		float z = light_temp.z - (light_temp.y*((light_temp.z - v_temp.z) / (light_temp.y- v_temp.y)));
		color = vNormal;
		gl_Position = projection*modelview*vec4(x,y,z,1.0);
	}
	else if(isShadow ==0)
	{
		if(islight == 1)
		{
			gl_Position = projection *((modelview* ctm * vPosition) + light_change);
			color = vec4(1,1,1,1);
		}
		else
		{
		ambient = AmbientProduct;
		vec4 N = normalize(modelview*ctm*vNormal);
		vec4 L_temp = modelview*(light_s - (ctm*vPosition)) + light_change;
		vec4 L = normalize(L_temp);
		diffuse = max(dot(L,N),0.0)*DiffuseProduct;
		vec4 Eyepoint = vec4(0.0,0.0,0.0,1.0);
		vec4 V = normalize(Eyepoint - (modelview*ctm*vPosition));
		vec4 H = normalize(L+V);
		specular = pow(max(dot(N,H),0.0),shininess)*SpecularProduct;
		float distance = length(L_temp);
		float attenuation = 1/(AttenuationConstant +(AttenuationLinear*distance)+(AttenuationQuadratic*distance*distance));
		color = ambient + (attenuation*(diffuse+specular));
		//color = N;
		//color = ambient;
		gl_Position = projection *modelview* ctm * vPosition;
		}

	}
}

