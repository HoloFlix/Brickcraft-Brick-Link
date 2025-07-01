#version 130
uniform sampler2D diffuseTexture;	//Rendered scene color values
uniform sampler2D depthTexture;		//Rendered scene depth values

uniform vec2 blurDirection; 		//Should be (1,0) or (0,1)
uniform vec2 screenResolution; 		//Screen resolution in pixels

uniform float focusedDepth;			//Depth of the center pixel as read from the depth buffer

/*
Current number of texture2D (sample) operations _per pass_:
2 (color + depth of current pixel) +
2 (depth of two other pixels nearby) + 
2 * numBlurPixelsPerSide (color for several nearby pixels for blur)

With two passes and a numBlurPixelsPerSide of 3, this gives:
(2 + 2 + (2 * 3)) * 2 = 20 texture samples
*/

in vec2 ex_texCoord;
out vec4 out_Color;

const float pi = 3.14159265;
const float numBlurPixelsPerSide = 5.0;

float LinearizeDepth(float depth)
{
  float n = 0.1; // camera z near
  float f = 500.0; // camera z far
  float z = depth;
  return (n * z) / ( f - z * (f - n) );
}

void main(void) {

	vec2 blurSize = blurDirection / screenResolution;
	
	float depth = LinearizeDepth(texture2D(depthTexture, ex_texCoord).x);
	
	float linearizedDepth = LinearizeDepth(focusedDepth);
	
	float maxNear = linearizedDepth;
	float minNear = max(0.0, maxNear - 1000.0 * linearizedDepth);
	float minFar = linearizedDepth;
	float maxFar = min(1.0, minFar + 150.0 * linearizedDepth);
		  
	float sigma = 7.0 * (min(maxNear - depth, maxNear - minNear) / (maxNear - minNear) + min(depth - minFar, maxFar - minFar) / (maxFar - minFar));
	
	/*
	//Edge detection
	float e1 = LinearizeDepth(texture2D(depthTexture, ex_texCoord - (1.0) * blurSize * 
						  blurDirection).x) / -2.0;
	float e2 = LinearizeDepth(texture2D(depthTexture, ex_texCoord + (1.0) * blurSize * 
						  blurDirection).x) / 2.0;
	
	//Some AA stuff
	float powDepth = pow(depth, 1.7);
	sigma += 15.0 * clamp(abs(e1 + e2), 0.0, powDepth) / powDepth;
	*/
	
	// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
	vec3 incrementalGaussian;
	incrementalGaussian.x = 1.0 / (sqrt(2.0 * pi) * sigma);
	incrementalGaussian.y = exp(-0.5 / (sigma * sigma));
	incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

	vec4 avgValue = vec4(0.0, 0.0, 0.0, 0.0);
	float coefficientSum = 0.0;

	// Take the central sample first...
	avgValue += texture2D(diffuseTexture, ex_texCoord) * incrementalGaussian.x;
	coefficientSum += incrementalGaussian.x;
	incrementalGaussian.xy *= incrementalGaussian.yz;

	//Sample depth a couple of pixels away in each direction - this should ideally be done per-pixel in the loop below,
	//but putting it here might be an acceptable optimization (even though it gives us some visual artifacts)
	float d1 = LinearizeDepth(texture2D(depthTexture, ex_texCoord - (numBlurPixelsPerSide + 1.0) * blurSize * 
						  blurDirection).x);
	float d2 = LinearizeDepth(texture2D(depthTexture, ex_texCoord + (numBlurPixelsPerSide + 1.0) * blurSize * 
						  blurDirection).x);
	float diff1 = min(abs(d1-linearizedDepth), 0.06) / 0.06;
	float diff2 = min(abs(d2-linearizedDepth), 0.06) / 0.06;	  
		
	// Go through the remaining (numBlurPixelsPerSide-1)*2 vertical samples of color (numBlurPixelsPerSide-1 on each side of the center)
	for (float i = 1.0; i <= numBlurPixelsPerSide; i++) {
		avgValue += texture2D(diffuseTexture, ex_texCoord - i * blurSize * 
							  blurDirection) * incrementalGaussian.x * diff1;
		avgValue += texture2D(diffuseTexture, ex_texCoord + i * blurSize * 
							  blurDirection) * incrementalGaussian.x * diff2;         
		coefficientSum += incrementalGaussian.x * (diff1 + diff2);
		incrementalGaussian.xy *= incrementalGaussian.yz;
	}

	gl_FragColor = avgValue / coefficientSum;
}