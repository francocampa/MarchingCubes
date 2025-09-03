#include "InteractiveMC.h"

void InteractiveMC::handleCamera(float delta)
{
	if (InputController::mouseRight) {
		//Rotate
		SDL_SetRelativeMouseMode(SDL_TRUE);
		float sensitivity = 0.5f;
		float theta = cam.theta + sensitivity * delta * InputController::mouseMove.x;
		float phi = cam.phi + sensitivity * sin(delta * InputController::mouseMove.y);

		float epsilon = 0.5f;
		if (phi < epsilon) phi = epsilon;
		if (phi > M_PI - epsilon) phi = M_PI - epsilon;

		cam.theta = theta;
		cam.phi = phi;
	}
	else if (InputController::mouseWheel) {
		float vel = 3.0f * delta;
		glm::vec3 front = glm::normalize(cam.position - center);
		glm::vec3 right = glm::normalize(glm::cross(front, { 0,1,0 }));
		glm::vec3 up = glm::normalize(glm::cross(right, front));

		right *= InputController::mouseMove.x * vel;
		up *= InputController::mouseMove.y * vel;

		cam.position += right + up;
	}
	else {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	if (!InputController::mouseWheel) {
		float vel = 10.0f;
		glm::vec3 front = glm::normalize(cam.position - center);
		glm::vec3 right = glm::normalize(glm::cross(front, { 0,1,0 }));
		if (InputController::up) {
			cam.position = cam.position - vel * delta * front;
		}
		if (InputController::down) {
			cam.position = cam.position + vel * delta * front;
		}
		if (InputController::right) {
			cam.position = cam.position - vel * delta * right;
		}
		if (InputController::left) {
			cam.position = cam.position + vel * delta * right;
		}
	}

	center.x = cam.position.x + sin(cam.phi) * cos(cam.theta);
	center.y = cam.position.y + cos(cam.phi);
	center.z = cam.position.z + sin(cam.phi) * sin(cam.theta);
}

void InteractiveMC::handleLight()
{
	if (enabledLight)
		return;

	glEnable(GL_LIGHT0);

	GLfloat lightPos[] = { 0.0f, 10.0f, 0.0f, 0.0f };
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	enabledLight = true;
}

void InteractiveMC::handleUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Files menu");
	ImGui::SliderFloat("Threshold", &threshold, 0.0f, 20.0f);
	for (int i = 0; i < 8; i++) {
		std::string label = "Vertex " + std::to_string(i);
		ImGui::SliderFloat(label.c_str(), &values[i], 0.0f, 20.0f);
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void InteractiveMC::process(float delta) {
	gluLookAt(cam.position.x, cam.position.y, cam.position.z, center.x, center.y, center.z, 0, 1, 0);

	handleCamera(delta);
	handleLight();

	//Render vertices
	for (int i = 0; i < 8; i++)
	{
		glm::vec3 color = values[i] > threshold ? glm::vec3(1, 1, 1) : glm::vec3(0, 0, 0);
		renderMesh(createSphereMesh(0.1f, 10, 10), getIndexPos(i), color, 1);
	}

	//Marching cubes alg 
	int mcCase = 0;
	for (int i = 0; i < 8; i++)
	{
		if (f(i) > threshold)
			mcCase |= (1 << i);
	}

	Mesh mc;
	for (int i = 0; i < 13; i = i + 3) {
		if (TriangleTable[mcCase][i] == -1)
			break;

		glm::vec3 points[3];
		for (int j = 0; j < 3; j++)
		{
			int edge = TriangleTable[mcCase][i + j];
			glm::vec3 corner0 = getIndexPos(EdgeVertexIndices[edge][0]);
			glm::vec3 corner1 = getIndexPos(EdgeVertexIndices[edge][1]);
			float corner0Val = f(EdgeVertexIndices[edge][0]);
			float corner1Val = f(EdgeVertexIndices[edge][1]);
			float t = (threshold - corner0Val) / (corner1Val - corner0Val);
			glm::vec3 point = corner0 + t * (corner1 - corner0);

			points[j] = point;
		}

		glm::vec3 e0 = points[1] - points[0];
		glm::vec3 e1 = points[2] - points[0];
		glm::vec3 normal = glm::cross(e1, e0);
		for (int j = 0; j < 3; j++) {
			Vertex v = { points[j], normal ,glm::vec2(0,0) };
			mc.indices.push_back(mc.vertices.size());
			mc.vertices.push_back(v);
		}
	}

	glEnable(GL_LIGHTING);

	renderMesh(mc, glm::vec3(0, 0, 0), glm::vec3(0.5, 0, 0), 1);

	glDisable(GL_LIGHTING);

	handleUI();

}


float InteractiveMC::f(int index)
{
	return values[index];
}

glm::vec3 InteractiveMC::getIndexPos(int i)
{
	glm::vec3 pos = {
			(i & 1) >> 0,
			(i & 2) >> 1,
			(i & 4) >> 2,
	};
	pos *= separation;
	return pos;
}
