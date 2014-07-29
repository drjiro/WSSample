#ifndef _SphereShaderNode2_H_
#define _SphereShaderNode2_H_

#include "cocos2d.h"
#include "Camera3d.h"

USING_NS_CC;

/**
 *	Vertex format
 */
struct VertexPositionNormalColorTexture
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat color[4];
	GLfloat texCoord[3];

	VertexPositionNormalColorTexture() {}
		VertexPositionNormalColorTexture(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;
		normal[0] = nx;
		normal[1] = ny;
		normal[2] = nz;
		color[0] = 1;
		color[1] = 1;
		color[2] = 1;
		color[3] = 1;
		texCoord[0] = u;
		texCoord[1] = v;
	}
};

class SphereShaderNode2 : public Node
{
public:
	static SphereShaderNode2* shaderNodeWithVertex(const std::string &vert, const std::string &frag, Texture2D* texture);

	virtual void update(float dt);
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

	void setCamera(Camera3d* camera);

protected:
	SphereShaderNode2();
	~SphereShaderNode2();

	bool initWithVertex(const std::string &vert, const std::string &frag, Texture2D* texture);
	void loadShaderVertex(const std::string &vert, const std::string &frag);

	void onDraw(const Mat4 &transform, uint32_t flags);

	float _time;
	float _rotY;

	Camera3d *_camera;

	VertexPositionNormalColorTexture* _vertices;
	GLushort* _indices;

	int _numIndices;
	int _numVertices;
	int _numFaces;

	std::string _vertFileName;
	std::string _fragFileName;
	CustomCommand _customCommand;
};

#endif