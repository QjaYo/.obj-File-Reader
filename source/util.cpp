#include "util.h"

#include <fstream>
#include <string>
#include <vector>

#include "object.h"
#include "global.h"

void Util::printDivider()
{
	printf("\n=====================================================\n\n");
}

bool Util::printTxtFile(const std::string& fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open())
	{
		std::cout << "Util::printTxtFile::Invalid File Name." << std::endl;
		return false;
	}
	std::string line;
	while (getline(fin, line))
	{
		std::cout << line << std::endl;
	}
	fin.close();
}

bool Util::loadObj(const std::string& fileName, std::vector<VertexData>& vertices)
{
	vertices.clear();
	std::vector<unsigned int> indices;
	std::vector<vec3> tmp_pos;
	std::vector<vec3> tmp_vNormal;

	std::ifstream fin(fileName);
	if (!fin.is_open())
	{
		std::cout << "Util::loadObj()::Invalid File Name." << std::endl;
		return false;
	}

	fin.seekg(0, std::ios::end);
	long long totalBytes = fin.tellg();
	fin.seekg(0, std::ios::beg);

	printf("Loading \"%s\"... 0%% ( 0 / %d )KB", fileName.c_str(), (int)totalBytes / 1024);

	std::string line;
	int lineNum = 1;
	long long currentBytes = 0;
	int lastPercent = -1;
	while (getline(fin, line))
	{
		currentBytes += line.size() + 1;
		int percent = (int)((double)currentBytes / totalBytes * 100.0);
		if (percent > lastPercent && percent % 5 == 0) // 5% 단위로 출력
		{
			printf("\rLoading \"%s\"... %d%% ( %d / %d )KB", fileName.c_str(), percent, (int)currentBytes / 1024, (int)totalBytes / 1024);
			lastPercent = percent;
		}
		//std::cout << line << std::endl;
		if (line.empty())
		{
			continue;
		}
		else if (line.rfind("v ", 0) == 0)
		{
			const char* p = line.c_str() + 2;
			float x = strtof(p, (char**)&p);
			float y = strtof(p, (char**)&p);
			float z = strtof(p, (char**)&p);
			tmp_pos.push_back(vec3(x, y, z));
			tmp_vNormal.push_back(vec3(0.0f, 0.0f, 0.0f));
		}
		else if (line.rfind("f ", 0) == 0)
		{
			const char* p = line.c_str() + 2;

			unsigned int index[3];
			for (int i = 0; i < 3; i++)
			{
				index[i] = strtol(p, (char**)&p, 10) - 1;
				indices.push_back(index[i]);

				VertexData v;
				{
					v.position = tmp_pos[index[i]];
					v.color = vec4(1.0f, 1.0f, 1.0f, 1.0f); // initial color: white
					v.vertexNormal = vec3(0.0f, 0.0f, 0.0f); // zero vector initialization
					v.faceNormal = vec3(0.0f, 0.0f, 0.0f); // zero vector initialization
				}
				vertices.push_back(v);
			}
		}
		lineNum++;
	}
	printf("\rLoading \"%s\"... 100%% ( %d / %d )KB\n", fileName.c_str(), (int)totalBytes / 1024, (int)totalBytes / 1024);
	fin.close();

	if (vertices.empty())
	{
		std::cout << "Util::loadObj()::The File is Empty." << std::endl;
		return false;
	}

	//move center to origin (0, 0, 0)
	vec3 minPos = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 maxPos = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (const auto& v : vertices)
	{
		if (v.position.x < minPos.x) minPos.x = v.position.x;
		if (v.position.y < minPos.y) minPos.y = v.position.y;
		if (v.position.z < minPos.z) minPos.z = v.position.z;

		if (v.position.x > maxPos.x) maxPos.x = v.position.x;
		if (v.position.y > maxPos.y) maxPos.y = v.position.y;
		if (v.position.z > maxPos.z) maxPos.z = v.position.z;
	}
	vec3 center = (minPos + maxPos) / 2.0f;
	for (auto& v : vertices)
	{
		v.position = v.position - center;
	}

	//size normalize (-0.5 ~ 0.5)
	float x_len = (maxPos.x - minPos.x);
	float y_len = (maxPos.y - minPos.y);
	float z_len = (maxPos.z - minPos.z);
	float max_len = std::max(x_len, std::max(y_len, z_len));
	if (max_len == 0) // prevent division by zero
		max_len = 1.0f;

	for (auto& v : vertices)
	{
		v.position.x = v.position.x / max_len;
		v.position.y = v.position.y / max_len;
		v.position.z = v.position.z / max_len;
	}

	// compute normal
	for (int i = 0; i < indices.size(); i += 3)
	{
		vec3 normal = normalize( cross( tmp_pos[indices[i + 1]] - tmp_pos[indices[i]], tmp_pos[indices[i + 2]] - tmp_pos[indices[i]] ) );

		for (int j = 0; j < 3; j++)
		{
			vertices[i + j].faceNormal = normal;
			tmp_vNormal[indices[i + j]] += normal;
		}
	}
	for (auto& n : tmp_vNormal)
		n = normalize(n);
	for (int i = 0; i < indices.size(); i++) // indices.size() == vertices.size()
		vertices[i].vertexNormal = tmp_vNormal[indices[i]];

	std::cout << "Util::loadObj()::File Loaded Successfully" << std::endl;
	return true;
}

mat4 Util::getProjMatrix(float fovy, float aspect, float zNear, float zFar)
{
	//perspective projection

	float rad = fovy * (PI / 180.0f);

	float h = 2 * zFar * tan(rad / 2);
	float w = aspect * h;
	mat4 S = Scale(2 / w, 2 / h, 1 / zFar); // zFar면을 width(-1~1), height(-1~1), z(-1)로 당겨오는 행렬

	float c = -zNear / zFar; // 당겨온 zNear면의 실제 z좌표

	mat4 M(1.0f);
	M[2] = vec4(0, 0, 1 / (c + 1), -c / (c + 1));
	M[3] = vec4(0, 0, -1, 0);

	return M * S;
}

mat4 Util::getViewMatrix(vec3 eye, vec3 at, vec3 up)
{
	mat4 viewMatrix = mat4(1.0f);

	up = normalize(up);
	vec3 cam_z = -normalize(at - eye);
	float alpha = dot(up, cam_z);
	vec3 cam_y = normalize(up - alpha * cam_z);
	vec3 cam_x = cross(cam_y, cam_z);

	viewMatrix[0] = vec4(cam_x, dot(-cam_x, eye));
	viewMatrix[1] = vec4(cam_y, dot(-cam_y, eye));
	viewMatrix[2] = vec4(cam_z, dot(-cam_z, eye));

	return viewMatrix;
}

mat4 Util::getModelMatrix(Object& obj)
{
	mat4 S = Scale(obj.scale.x, obj.scale.y, obj.scale.z);
	mat4 R = RotateX(obj.rotation.x) * RotateY(obj.rotation.y) * RotateZ(obj.rotation.z);
	mat4 T = Translate(obj.position);
	
	return T * R * S;
}

//mat4 Util::getProjMatrix_ortho()
//{
//
//}