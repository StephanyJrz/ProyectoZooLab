/*---------------------------------------------------------------------*/
/* -------------------------   Proyecto Final   -----------------------*/
/*-----------------------------    2023-2   ---------------------------*/
/*----------- Alumnos ---------------------- No. Cuenta ---------------*/
/* Conde Barrios Luis Leonardo.              317051016  ---------------*/
/* Juárez Gallegos Stephany Anali.           317148868  ---------------*/
/* Ramirez Sanchez Jose Roberto              314127093  ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(60.0f, 10.0f, 20.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//Variables animacion jirafa
float	mueveCuello = 0.0f;
bool	cuelloHaciaAbajo = true,
		cuelloHaciaArriba = false;

//leon animacion
float	movleon_x = -260.0f,
		movleon_z = 320.0f,
		movleon_y = 15.0f,
		ori = 0.0f;

bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;

float myVariable = 0.0f;
bool vuelo = false;
bool avanza = false;
bool baja = false;

//Variables animacion rino
float	movrino_x = -280.0f,
		rotaRino = 0.0f;
bool	recoRinoUno = true,
		recoRinoDos = false;

//Variables animacion mono
float	movmono_y = 2.0f; 

bool subeMono = true,
	 bajaMono = false;
// variables de animacion del ave
float movAlto_y = 2.0f,
	movBajo_y,
	  movAdeX=-300.0f;
bool vuelaAlto = true,
	vuelaBajo=false,
	adelante=false,
	atras=false;


//Keyframes (Manipulación y dibujo)
float	movimientoX = 0.0f,
		movimientoZ = 0.0f,
		orientacion = 0.0f;

float	incrementoX = 0.0f,
		incrementoZ = 0.0f,
		incOrientacion = 0.0f;

#define MAX_FRAMES 14
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movimientoX;	//posX
	float movimientoZ;	//posZ
	float orientacion;  //giroMonito

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 14;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].movimientoX = movimientoX;
	KeyFrame[FrameIndex].movimientoZ = movimientoZ;
	KeyFrame[FrameIndex].orientacion = orientacion;

	FrameIndex++;
}

void resetElements(void)
{
	movimientoX = KeyFrame[0].movimientoX;
	movimientoZ = KeyFrame[0].movimientoZ;
	orientacion = KeyFrame[0].orientacion;
}

void interpolation(void)
{
	incrementoX = (KeyFrame[playIndex + 1].movimientoX - KeyFrame[playIndex].movimientoX) / i_max_steps;
	incrementoZ = (KeyFrame[playIndex + 1].movimientoZ - KeyFrame[playIndex].movimientoZ) / i_max_steps;
	incOrientacion = (KeyFrame[playIndex + 1].orientacion - KeyFrame[playIndex].orientacion) / i_max_steps;
}


void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			movimientoX += incrementoX;
			movimientoZ += incrementoZ;
			orientacion += incOrientacion;

			i_curr_steps++;
		}
	}

	//Animacion jirafa-------------
	if (cuelloHaciaAbajo) {
		mueveCuello += 0.3f;
		if (mueveCuello >= 25.0f) {
			cuelloHaciaAbajo = false;
			cuelloHaciaArriba = true;
		}
	}

	if (cuelloHaciaArriba) {
		mueveCuello -= 0.3f;
		if (mueveCuello <= 20.0f) {
			cuelloHaciaAbajo = true;
			cuelloHaciaArriba = false;
		}
	}
	//---------------------------------Leon
	if (vuelo) {
		movleon_y += 1.5;
		if (movleon_y >= 18.0f) {
				vuelo = false;
				avanza = true;
		}
	}
	if (avanza) {
		movleon_z -= 1.5f;
		if (movleon_z <= 250.0f) {
			avanza = false;
			baja = true;
		}
	}
	if (baja) {
		movleon_y -= 1.5f;
		if (movleon_y <= 15.0f) {
			baja = false;
			avanza = false;
		}
	}

	//Animacion rino
	if (recoRinoUno) {
		movrino_x += 1.0f;
		if (movrino_x >= -230.0f) {
			recoRinoUno = false;
			recoRinoDos = true;
		}
	}
	if (recoRinoDos) {
		movrino_x -= 1.0f;
		if (movrino_x <= -290.0f) {
			recoRinoUno = true;
			recoRinoDos = false;
		}
	}

	//Animacion mono
	if (subeMono) {
		movmono_y += 0.25f;
		if (movmono_y >= 25.0f) {
			subeMono = false;
			bajaMono = true;
		}
	}
	if (bajaMono) {
		movmono_y -= 0.25f;
		if (movmono_y <= 2.0f) {
			subeMono = true;
			bajaMono = false;
		}
	}
	//animacio ave
	if (vuelaAlto) {
		movAlto_y += 1.0;
		movAdeX += 0.5f;
		if (movAlto_y > 50.0f) {
			vuelaAlto = false;
			vuelaBajo = true;
		}

	}
	if (vuelaBajo) {
		movAlto_y -= 1.0;
		movAdeX -= 0.5f;
		if (movAlto_y < 0.0) {
			vuelaBajo = false;
			vuelaAlto = true;
		}
	}
	

}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model pisoZoo("resources/objects/piso/pisoZoo.obj");
	Model paredHabitat("resources/objects/paredes/paredesHabitat.obj");
	Model banio("resources/objects/paredes/banio.obj");
	Model toilet("resources/objects/paredes/toilet.obj");
	Model lavabo("resources/objects/paredes/lavaboBanio.obj");
	Model mesa("resources/objects/banca/mesa.obj");
	Model mostrador("resources/objects/tienda/mostrador.obj");
	Model tienda("resources/objects/tiendaRegalos/tiendaRegalos.obj");
	Model paradaVehiculo("resources/objects/paradaVehiculo/paradaVehiculo.obj");

	Model guacamaya("resources/objects/guacamaya/guacamaya.obj");
	Model umbrella("resources/objects/umbrella/umbrella.obj");
	Model menu("resources/objects/menu/menu.obj");
	Model jeep("resources/objects/safariJeep/safariJeep.obj");
	Model publiEntrada("resources/objects/publicidad/publiEntrada.obj");
	Model publiParking("resources/objects/publicidad/publiParking.obj");
	Model roca("resources/objects/paredes/roca.obj");
	Model rhino("resources/objects/rhino/rhinos.obj");
	Model arbol("resources/objects/arbol/arbol.obj");
	Model jirafa("resources/objects/jirafa/jirafa.obj");
	Model cuerpoAnim("resources/objects/jirafa/cuerpoAnim.obj");//Cuerpo de la jirafa animada
	Model cuelloAnim("resources/objects/jirafa/cuelloAnim.obj");//Cuello de la jirafa animada
	Model leon("resources/objects/leon/leone.obj");
	Model armon("resources/objects/armon/Tree.obj");
	Model Mono("resources/objects/mono/mono.obj");
	Model jaula("resources/objects/jaula/jaula.obj");
	Model limona("resources/objects/limona/12232_amazon_parrot_v1_L2.obj");
	Model ave("resources/objects/limona/ave.obj");
	Model auto1("resources/objects/auto/auto1.obj");
	
	ModelAnim cocinera("resources/objects/cocinera/BriefcaseIdle.dae");
	cocinera.initShaders(animShader.ID);

	ModelAnim cajero("resources/objects/cajero/ArmStretching.dae");
	cajero.initShaders(animShader.ID);
	
															
	//Inicialización de KeyFrames
	
	KeyFrame[0].movimientoX = 0.0f;
	KeyFrame[0].movimientoZ = 0.0f;
	KeyFrame[0].orientacion = 0.0f;

	KeyFrame[1].movimientoX = -400.0f;
	KeyFrame[1].movimientoZ = 0.0f;
	KeyFrame[1].orientacion = 0.0f;

	KeyFrame[2].movimientoX = -400.0f;
	KeyFrame[2].movimientoZ = -500.0f;
	KeyFrame[2].orientacion = -90.0f;

	KeyFrame[3].movimientoX = -90.0f;
	KeyFrame[3].movimientoZ = -500.0f;
	KeyFrame[3].orientacion = 180.0f;

	KeyFrame[4].movimientoX = -90.0f;
	KeyFrame[4].movimientoZ = 270.0f;
	KeyFrame[4].orientacion = 90.0f;

	KeyFrame[5].movimientoX = -400.0f;
	KeyFrame[5].movimientoZ = 270.0f;
	KeyFrame[5].orientacion = 0.0f;

	KeyFrame[6].movimientoX = -400.0f;
	KeyFrame[6].movimientoZ = 0.0f;
	KeyFrame[6].orientacion = -90.0f;

	KeyFrame[7].movimientoX = -100.0f;
	KeyFrame[7].movimientoZ = 0.0f;
	KeyFrame[7].orientacion = 0.0f;

	

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		//Matrices temporales para animaciones
		//Jirafa
		glm::mat4 tempJirafa = glm::mat4(1.0f);
		//Guacamaya
		glm::mat4 tempGuacamaya = glm::mat4(1.0f);
		
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personajes Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, -1.7f, -330.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.15f));	// it's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		cocinera.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(37.0f, 0.5f, -360.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.17f));	// it's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		cajero.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//Colocación de piso
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		staticShader.setMat4("model", model);
		pisoZoo.Draw(staticShader);

		//Colocación de hábitats---------------------------------------------------
		//Hábitat más lejano-------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-260.0f, -5.2f, -280.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.7f, 1.2f));
		staticShader.setMat4("model", model);
		paredHabitat.Draw(staticShader);

		//jaula para aves
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-290.0f, -5.2f, -150.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.7f, 1.2f));
		staticShader.setMat4("model", model);
		jaula.Draw(staticShader);
		//armon
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-250.0f, 0.0f, -140.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		armon.Draw(staticShader);
		//perico
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-250.5f, 10.0f, -137.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		limona.Draw(staticShader);
		//ave
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movAdeX, movAlto_y, -127.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		ave.Draw(staticShader);
		//Rhino
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movrino_x, -1.0 , -235.0f));
		model = glm::scale(model, glm::vec3(40.0f));
		model = glm::rotate(model, glm::radians(rotaRino), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		rhino.Draw(staticShader);
		//Mono
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-257.5f, movmono_y, -288.4f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		Mono.Draw(staticShader);
		//armon
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-260.0f, 2.0f, -292.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		armon.Draw(staticShader);
		//arbol
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-270.0f, 5.0f, -230.0f));
		model = glm::scale(model, glm::vec3(150.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);
		//--------------------------------------------------------------------------
		//Hábitat medio-------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-260.0f, -5.2f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.7f, 1.2f));
		staticShader.setMat4("model", model);
		paredHabitat.Draw(staticShader);
		//jirafa estatica
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-260.0f, 4.0f, 20.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		jirafa.Draw(staticShader);
		//jirafa animada cuerpo
		model = glm::mat4(1.0f);
		tempJirafa = model = glm::translate(model, glm::vec3(-260.0f, 4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		cuerpoAnim.Draw(staticShader);
		//jirafa animada cuello
		model = glm::translate(tempJirafa, glm::vec3(130.0f, 65.0f, 142.0f));
		model = glm::rotate(model, glm::radians(mueveCuello),glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(15.0f, -10.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		cuelloAnim.Draw(staticShader);
		//arbol
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-275.0f, 4.0f,-20.0f));
		model = glm::scale(model, glm::vec3(70.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);
		//------------------------------------------------------------------------
		//Hábitat más cercano
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-260.0f, -5.2f, 280.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.7f, 1.2f));
		staticShader.setMat4("model", model);
		paredHabitat.Draw(staticShader);
		//guacamayas estaticas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-310.0f, 4.0f, 320.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		staticShader.setMat4("model", model);
		guacamaya.Draw(staticShader);
		//arbol
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-310.0f, 4.0f, 280.0f));
		model = glm::scale(model, glm::vec3(160.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);
		//Roca uno para leon
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-250.0f, 4.0f,320.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		roca.Draw(staticShader);
		//leon
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(15.0f + movleon_x, -1.0 + movleon_y, movleon_z));
		//model = glm::translate(model, glm::vec3(-250.0f, 15.0f, 320.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		leon.Draw(staticShader);
		//Roca dos para leon
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-250.0f, 4.0f, 250.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		roca.Draw(staticShader);
		//guacamaya animada
		/*Cuerpo
		model = glm::mat4(1.0f);
		tempGuacamaya = model = glm::translate(model, glm::vec3(-280.0f, 31.0f, 280.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		staticShader.setMat4("model", model);
		cuerpoGua.Draw(staticShader);
		//Cabeza
		model = glm::translate(tempGuacamaya, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mueveCuello), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		staticShader.setMat4("model", model);
		cabezaGua.Draw(staticShader);
		//Alas
		model = glm::translate(tempGuacamaya, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		staticShader.setMat4("model", model);
		alasGua.Draw(staticShader);*/
		//Paredes entrada----------------------------------------------------------- 
		//Más lejana uno
		//Paredes entrada----------------------------------------------------------- 
		//Más lejana uno
		
		
		//Publicidad entrada
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(135.0f, -1.0f, -290.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		publiEntrada.Draw(staticShader);
		
		//Publicidad estacionamiento Uno
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(135.0f, -1.0f, 120.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		publiParking.Draw(staticShader);
		//Publicidad estacionamiento Dos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(135.0f, -3.0f, 320.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		publiEntrada.Draw(staticShader);
		//autoas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(205.0f, 1.0f, 150.0f));
		model = glm::scale(model, glm::vec3(12.0f));
		staticShader.setMat4("model", model);
		auto1.Draw(staticShader);

		//Instalaciones---------------------------------------------------------------
		//Banio
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-27.0f, -1.5f, 380.0f));
		model = glm::scale(model, glm::vec3(1.6f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		banio.Draw(staticShader);
		//Toilet
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-45.0f, -1.5f, 396.0f));
		model = glm::scale(model, glm::vec3(2.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		toilet.Draw(staticShader);
		//Lavabo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-40.0f, -7.2f, 364.0f));
		model = glm::scale(model, glm::vec3(18.1f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lavabo.Draw(staticShader);
		//Tienda de regalos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, -1.5f, 376.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		tienda.Draw(staticShader);
		//Parada de vehiculo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 150.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		paradaVehiculo.Draw(staticShader);
		//Jeep
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movimientoX, -1.5f, 120.0f + movimientoZ));
		model = glm::scale(model, glm::vec3(14.2f));
		model = glm::rotate(model, glm::radians(270.0f + orientacion), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		jeep.Draw(staticShader);

		//Zona de comida
		// Mesas con bancas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(35.0f, -1.5f, -270.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, -1.5f, -270.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-57.0f, -1.5f, -300.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-57.0f, -1.5f, -340.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		//mostrador 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(37.0f, 0.0f, -330.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mostrador.Draw(staticShader);

		//sombrilla
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-30.0f, -2.0f, -312.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		umbrella.Draw(staticShader);

		//menu
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, -225.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		menu.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		movimientoX--;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		movimientoX++;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		movimientoZ++;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		movimientoZ--;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		orientacion++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		orientacion--;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	//leon animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		vuelo = true;
		animacion ^= true; //originalmente es un xor || animacion = !animacion;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		movleon_z = 320.0f;
		movleon_y = 15.0f;
	}
	
	
	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}