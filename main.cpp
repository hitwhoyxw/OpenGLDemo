#include <iostream>		
using namespace std;
#include <GL/glew.h>    
#include <GLFW/glfw3.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include"Shader.h"
#include<time.h>
#include <vector>

// 定义顶点着色器
const char* vertexShaderSource = "vertexShader.glsl";
// 定义片段着色器
const char* fragmentShaderSource = "fragmentShader.glsl";
GLuint VBO, VAO, EBO;
GLint WIDTH = 800, HEIGHT = 600;		// 先设置窗口以及其大小
void ReShape(GLFWwindow* window, int w, int h);
void SetVetexData(GLuint& VAO, GLuint& VBO, GLuint& EBO, GLfloat* vertices, GLuint vertexCount, GLuint* indices, GLuint indiceCount);
void Draw(GLuint VAO, GLFWwindow* window_1);
void ComputePositionOffsets(float& fXOffset, float& fYOffset);
void AdjustVertexData(float fXOffset, float fYOffset);
//顶点数据，三个点 然后三个点的颜色
float vertices[] = {
	 0.0f,    0.5f, 0.0f, 1.0f,
	0.5f, -0.366f, 0.0f, 1.0f,
	-0.5f, -0.366f, 0.0f, 1.0f,
	1.0f,    0.0f, 0.0f, 1.0f,
	0.0f,    1.0f, 0.0f, 1.0f,
	0.0f,    0.0f, 1.0f, 1.0f,
};
GLuint indices[] = {  // 注意索引从0开始! 
		0, 1, 2,  // 第一个三角形
};
int main()
{
	glfwInit();								//初始化，使用glfw来打开一个窗口
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, true);						
	GLFWwindow* window_1 = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	//glfwGetFramebufferSize(window_1, &screenWidth, &screenHeight);  // 获得实际占用的帧缓存大小
	glfwSetFramebufferSizeCallback(window_1, ReShape);	// 设置窗口大小改变时的回调函数
	if (nullptr == window_1)			//判断窗口输出是否成功
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);	// 设置视口的大小
	glfwMakeContextCurrent(window_1);   // 捕获当前窗口，准备对当前窗口进行画图
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)			// 判断glew初始化是否成功
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
	glDeleteVertexArrays(1, &VAO);		// 删除VAO
	glDeleteBuffers(1, &VBO);			// 删除VBO
	glDeleteBuffers(1, &EBO);			// 删除EBO
	glfwTerminate();								// 如果循环结束：glfw关闭
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
	//顶点属性对象，记录glVertexAttribPointer等设置的信息，会记录绑定的VBO
	glGenVertexArrays(1, &VAO);
	//顶点缓冲对象，记录顶点数据
	glGenBuffers(1, &VBO);
	//索引缓冲对象，记录索引数据
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);		// 绑定VAO,在glVertexAttribPointer调用前需要绑定到上下文

	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// 绑定VBO
	glBufferData(GL_ARRAY_BUFFER, vertexCount, vertices, GL_STREAM_DRAW);	// 将顶点数据复制到缓冲中

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	// 绑定EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceCount, indices, GL_STATIC_DRAW);	// 将索引数据复制到缓冲中

	glBindVertexArray(0);		// 解绑VAO
}
void Draw(GLuint VAO,GLFWwindow* window_1)
{
	float fXOffset = 0.0f, fYOffset = 0.0f;
	ComputePositionOffsets(fXOffset, fYOffset);
	AdjustVertexData(fXOffset, fYOffset);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// 分别是红、绿、蓝、透明度的四个参数。RGB三原色+透明度
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAO);		// 绑定VAO
	//attributeIndex =0, dimension = 3,isNormalLized = GL_FALSE, offset = 3 * sizeof(GLfloat), pointer = (GLvoid*)0 因为传的是数组，退化为指针，shader实际用的是vec
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);	// 设置顶点属性指针,告诉怎么读取，内部会找绑定的VBO和EBO记录下来
	glEnableVertexAttribArray(0);	// 启用顶点属性
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(3*4*sizeof(float)));	// 设置顶点属性指针,告诉怎么读取，内部会找绑定的VBO和EBO记录下来
	glEnableVertexAttribArray(1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);	// 绘制三角形
	glfwSwapBuffers(window_1);					// 双缓存模式
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
	//不能解绑，会找不到
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}