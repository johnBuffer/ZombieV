uniform sampler2D texture;
uniform float BLUR_FACTOR;
uniform float MAX_HEIGHT;

uniform float DIRECTION;

uniform float WIDTH;
uniform float HEIGHT;

vec4 weight = vec4(0.006, 0.061, 0.242, 0.383);

float WIDTH_STEP = 1.0/WIDTH;
float HEIGHT_STEP= 1.0/HEIGHT;

void main()
{
	float VDIR = DIRECTION==1?0:1;
	vec2 pos = vec2(gl_FragCoord.x*WIDTH_STEP, gl_FragCoord.y*HEIGHT_STEP);
	vec2 offset = vec2(WIDTH_STEP*DIRECTION, HEIGHT_STEP*VDIR);
	
	float xStep = floor(gl_FragCoord.x);
	float yStep = floor(gl_FragCoord.y);
		
	vec4 color = texture2D(texture, pos) * weight[3];
	
	color += texture2D(texture, pos+offset) * weight[2];
	color += texture2D(texture, pos+offset*2) * weight[1];
	color += texture2D(texture, pos+offset*3) * weight[0];
	
	color += texture2D(texture, pos-offset) * weight[2];
	color += texture2D(texture, pos-offset*2) * weight[1];
	color += texture2D(texture, pos-offset*3) * weight[0];
	
	gl_FragData[0] = vec4(color.xyz, 1.0);

}