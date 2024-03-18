#include "ModelRenderer.h"

#include "Util.cpp"

ModelRenderer::ModelRenderer(FPSCamera * camera)
{
	this->camera = camera;
}

ModelRenderer::~ModelRenderer(){}

void ModelRenderer::init()
{
	programId = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	string fileText = Util::readFile("vertexshader.shader");
	const GLchar * shaderSource = fileText.c_str();

	glShaderSource(vertexShader, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(vertexShader);
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);

	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		Util::printShaderLog(vertexShader);
		return;
	}

	glAttachShader(programId, vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	fileText = Util::readFile("fragmentshader.shader");
	shaderSource = fileText.c_str();

	glShaderSource(fragmentShader, 1, (const GLchar**)&shaderSource, NULL);

	glCompileShader(fragmentShader);

	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile fragment shader %d!\n", fragmentShader);
		return;
	}

	glAttachShader(programId, fragmentShader);


	glLinkProgram(programId);

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", programId);
		return;
	}

	vertexPositionLocation = glGetAttribLocation(programId, "vertexPosition");

	if (vertexPositionLocation == -1)
	{
		printf("vertexPos2D is not a valid glsl program variable!\n");
		return;
	}

	uvLocation = glGetAttribLocation(programId, "uv");

	if (uvLocation == -1)
	{
		printf("uv is not a valid glsl program variable!\n");
		return;
	}

	modelMatrixLocation = glGetUniformLocation(programId, "modelMatrix");
	viewMatrixLocation = glGetUniformLocation(programId, "viewMatrix");
	projectionMatrixLocation = glGetUniformLocation(programId, "projectionMatrix");
	textureSamplerLocation = glGetUniformLocation(programId, "textureSampler");
}

void ModelRenderer::renderModel(Model * m)
{

	glUseProgram(programId);
	
	glm::vec3 translation = m->position;
	glm::vec3 rotation = m->rotation;
	glm::vec3 scale = m->scale;
	
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, rotation.x * 180.0f / 3.14159265f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.y * 180.0f / 3.14159265f, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.z * 180.0f / 3.14159265f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, translation);
	modelMatrix = glm::scale(modelMatrix, scale);
	
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));
	
	glEnableVertexAttribArray(vertexPositionLocation);


	glBindBuffer(GL_ARRAY_BUFFER, m->vbos[0]);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

	glEnableVertexAttribArray(uvLocation);
	glBindBuffer(GL_ARRAY_BUFFER, m->uvbos[0]);
	glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureSamplerLocation, 0);
	glBindTexture(GL_TEXTURE_2D, m->texture->textureId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibos[0]);
	
	glDrawElements(GL_TRIANGLES, m->indexCounts[0], GL_UNSIGNED_INT, NULL);
	
	glDisableVertexAttribArray(vertexPositionLocation);
	
	glUseProgram(NULL);
}

void ModelRenderer::renderTerrain(Terrain * t)
{
	glUseProgram(programId);

	glm::vec3 translation = glm::vec3(-512, -100, -512);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, rotation.x * 180.0f / 3.14159265f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.y * 180.0f / 3.14159265f, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.z * 180.0f / 3.14159265f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, translation);
	modelMatrix = glm::scale(modelMatrix, scale);

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));

	glEnableVertexAttribArray(vertexPositionLocation);

	glBindBuffer(GL_ARRAY_BUFFER, t->vertexBufferObject);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

	glEnableVertexAttribArray(uvLocation);
	glBindBuffer(GL_ARRAY_BUFFER, t->uvBufferObject);
	glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureSamplerLocation, 0);
	glBindTexture(GL_TEXTURE_2D, t->texture->textureId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t->indexBufferObject);

	glDrawElements(GL_TRIANGLES, t->indexCount, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(vertexPositionLocation);

	glUseProgram(NULL);
}

void ModelRenderer::renderSkydome(Skydome * s)
{
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glUseProgram(programId);


	glm::vec3 translation = glm::vec3(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, translation);
	modelMatrix = glm::rotate(modelMatrix, rotation.x * 180.0f / 3.14159265359f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.y * 180.0f / 3.14159265359f, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.z * 180.0f / 3.14159265359f, glm::vec3(0.0f, 0.0f, 1.0f));

	modelMatrix = glm::scale(modelMatrix, scale);

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));

	glEnableVertexAttribArray(vertexPositionLocation);

	glBindBuffer(GL_ARRAY_BUFFER, s->vertexBufferObject);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

	glEnableVertexAttribArray(uvLocation);
	glBindBuffer(GL_ARRAY_BUFFER, s->uvBufferObject);
	glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureSamplerLocation, 0);
	glBindTexture(GL_TEXTURE_2D, s->texture->textureId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->indexBufferObject);

	glDrawElements(GL_TRIANGLES, s->indexCount, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(vertexPositionLocation);

	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

	glUseProgram(NULL);

}

void ModelRenderer::renderBillboard(Billboard * b)
{

	glUseProgram(programId);

	glm::vec3 translation = b->position;
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = b->scale;

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, rotation.x * 180.0f / 3.14159265f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians((360.0f - camera->yaw) + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.z * 180.0f / 3.14159265f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, translation);
	modelMatrix = glm::scale(modelMatrix, scale);


	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));

	glEnableVertexAttribArray(vertexPositionLocation);

	glBindBuffer(GL_ARRAY_BUFFER, b->vertexBufferObject);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

	glEnableVertexAttribArray(uvLocation);
	glBindBuffer(GL_ARRAY_BUFFER, b->uvBufferObject);
	glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureSamplerLocation, 0);
	glBindTexture(GL_TEXTURE_2D, b->texture->textureId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b->indexBufferObject);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(vertexPositionLocation);

	glUseProgram(NULL);
}

void ModelRenderer::renderText(GUIText * t)
{
	glUseProgram(programId);
	glDisable(GL_CULL_FACE);
	glm::vec3 translation = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(100.0, 100.0, 100.0);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, rotation.x * 180.0f / 3.14159265f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.y * 180.0f / 3.14159265f, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.z * 180.0f / 3.14159265f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, translation);
	modelMatrix = glm::scale(modelMatrix, scale);

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));

	glEnableVertexAttribArray(vertexPositionLocation);

	glBindBuffer(GL_ARRAY_BUFFER, t->vertexBufferObject);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

	glEnableVertexAttribArray(uvLocation);
	glBindBuffer(GL_ARRAY_BUFFER, t->uvBufferObject);
	glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureSamplerLocation, 0);
	glBindTexture(GL_TEXTURE_2D, t->font->texture->textureId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t->indexBufferObject);

	glDrawElements(GL_TRIANGLES, t->indexCount, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(vertexPositionLocation);

	glUseProgram(NULL);
}