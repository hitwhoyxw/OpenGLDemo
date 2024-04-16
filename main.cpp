#include <iostream>		
using namespace std;
#include <GL/glew.h>    
#include <GLFW/glfw3.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include"Shader.h"
#include<time.h>
#include <vector>

// ���嶥����ɫ��
const char* vertexShaderSource = "vertexShader.glsl";
// ����Ƭ����ɫ��
const char* fragmentShaderSource = "fragmentShader.glsl";
GLuint VBO, VAO, EBO;
GLint WIDTH = 800, HEIGHT = 600;		// �����ô����Լ����С
void ReShape(GLFWwindow* window, int w, int h);
void SetVetexData(GLuint& VAO, GLuint& VBO, GLuint& EBO, GLfloat* vertices, GLuint vertexCount, GLuint* indices, GLuint indiceCount);
void Draw(GLuint VAO, GLFWwindow* window_1);
void ComputePositionOffsets(float& fXOffset, float& fYOffset);
void AdjustVertexData(float fXOffset, float fYOffset);
//�������ݣ������� Ȼ�����������ɫ
float vertices[] = {
	 0.0f,    0.5f, 0.0f, 1.0f,
	0.5f, -0.366f, 0.0f, 1.0f,
	-0.5f, -0.366f, 0.0f, 1.0f,
	1.0f,    0.0f, 0.0f, 1.0f,
	0.0f,    1.0f, 0.0f, 1.0f,
	0.0f,    0.0f, 1.0f, 1.0f,
};
GLuint indices[] = {  // ע��������0��ʼ! 
		0, 1, 2,  // ��һ��������
};
int main()
{
	glfwInit();								//��ʼ����ʹ��glfw����һ������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, true);						
	GLFWwindow* window_1 = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	//glfwGetFramebufferSize(window_1, &screenWidth, &screenHeight);  // ���ʵ��ռ�õ�֡�����С
	glfwSetFramebufferSizeCallback(window_1, ReShape);	// ���ô��ڴ�С�ı�ʱ�Ļص�����
	if (nullptr == window_1)			//�жϴ�������Ƿ�ɹ�
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);	// �����ӿڵĴ�С
	glfwMakeContextCurrent(window_1);   // ����ǰ���ڣ�׼���Ե�ǰ���ڽ��л�ͼ
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)			// �ж�glew��ʼ���Ƿ�ɹ�
	{
		cout << "Failed to initialise GLEW" << endl;
		glfwTerminate();
		return -1;
	}

	Shader shader(vertexShaderSource, fragmentShaderSource);
	shader.use();


	SetVetexData(VAO, VBO, EBO, vertices, sizeof(vertices), indices, sizeof(indices));

	while (!glfwWindowShouldClose(window_1))
	{
		glfwPollEvents();
		Draw(VAO, window_1);
	}
	glDeleteVertexArrays(1, &VAO);		// ɾ��VAO
	glDeleteBuffers(1, &VBO);			// ɾ��VBO
	glDeleteBuffers(1, &EBO);			// ɾ��EBO
	glfwTerminate();								// ���ѭ��������glfw�ر�
	return 0;
}


void ReShape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	WIDTH = w;
	HEIGHT = h;
}
void SetVetexData(GLuint& VAO, GLuint& VBO, GLuint& EBO, GLfloat* vertices, GLuint vertexCount, GLuint* indices, GLuint indiceCount)
{
	//�������Զ��󣬼�¼glVertexAttribPointer�����õ���Ϣ�����¼�󶨵�VBO
	glGenVertexArrays(1, &VAO);
	//���㻺����󣬼�¼��������
	glGenBuffers(1, &VBO);
	//����������󣬼�¼��������
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);		// ��VAO,��glVertexAttribPointer����ǰ��Ҫ�󶨵�������

	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// ��VBO
	glBufferData(GL_ARRAY_BUFFER, vertexCount, vertices, GL_STREAM_DRAW);	// ���������ݸ��Ƶ�������

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	// ��EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceCount, indices, GL_STATIC_DRAW);	// ���������ݸ��Ƶ�������

	glBindVertexArray(0);		// ���VAO
}
void Draw(GLuint VAO,GLFWwindow* window_1)
{
	float fXOffset = 0.0f, fYOffset = 0.0f;
	ComputePositionOffsets(fXOffset, fYOffset);
	AdjustVertexData(fXOffset, fYOffset);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// �ֱ��Ǻ졢�̡�����͸���ȵ��ĸ�������RGB��ԭɫ+͸����
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAO);		// ��VAO
	//attributeIndex =0, dimension = 3,isNormalLized = GL_FALSE, offset = 3 * sizeof(GLfloat), pointer = (GLvoid*)0 ��Ϊ���������飬�˻�Ϊָ�룬shaderʵ���õ���vec
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);	// ���ö�������ָ��,������ô��ȡ���ڲ����Ұ󶨵�VBO��EBO��¼����
	glEnableVertexAttribArray(0);	// ���ö�������
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(3*4*sizeof(float)));	// ���ö�������ָ��,������ô��ȡ���ڲ����Ұ󶨵�VBO��EBO��¼����
	glEnableVertexAttribArray(1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);	// ����������
	glfwSwapBuffers(window_1);					// ˫����ģʽ
}

void ComputePositionOffsets(float& fXOffset, float& fYOffset)
{
	const float fLoopDuration = 5.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fElapsedTime =  clock()/ 1000.0f;

	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.5f;
	fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.5f;
}

void AdjustVertexData(float fXOffset, float fYOffset)
{
	GLuint vertexSize = sizeof(vertices);
	std::vector<float> fNewData(vertexSize);
	memcpy(&fNewData[0], vertices, vertexSize);

	for (int iVertex = 0; iVertex < vertexSize; iVertex += 4)
	{
		fNewData[iVertex] += fXOffset;
		fNewData[iVertex + 1] += fYOffset;
	}

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, &fNewData[0]);
	//���ܽ�󣬻��Ҳ���
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}