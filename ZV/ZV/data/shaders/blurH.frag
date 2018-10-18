uniform sampler2D texture;

uniform float WIDTH;
uniform float HEIGHT;
uniform float SCALE;

vec4 weight = vec4(0.006, 0.061, 0.242, 0.383);

float WIDTH_STEP = 1.0/(WIDTH*SCALE);
float HEIGHT_STEP= 1.0/(HEIGHT*SCALE);

void main()
{
	vec2 pos = vec2(gl_TexCoord[0].x, gl_TexCoord[0].y);
	vec2 offset = vec2(WIDTH_STEP, 0);
	
	vec4 color = texture2D(texture, pos) * weight[3];
	
	color += texture2D(texture, pos+offset)   * weight[2];
	color += texture2D(texture, pos+offset*2) * weight[1];
	color += texture2D(texture, pos+offset*3) * weight[0];
	
	color += texture2D(texture, pos-offset)   * weight[2];
	color += texture2D(texture, pos-offset*2) * weight[1];
	color += texture2D(texture, pos-offset*3) * weight[0];
	
	gl_FragData[0] = vec4(color.xyz, 1.0);
		
	//gl_FragData[0] = texture2D(texture, gl_TexCoord[0]);//vec4(vec3(gl_TexCoord[0].x*WIDTH_STEP, gl_TexCoord[0].y*HEIGHT_STEP, 0.0), 1.0);
}