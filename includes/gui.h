#ifndef IHM_H
# define IHM_H
# include <string>

	# ifndef M_PI
		const double M_PI = (double)3.1415926535;
	#endif

	typedef struct s_Color
	{
		double r;
		double g;
		double b;
	}t_Color;

	int WIDTH = 800;
	int HEIGHT = 600;
	float ASPECT = (float)WIDTH / HEIGHT;
	std::string title = "2048";
	std::string scoreTitle = "Score:";
	bool isInsideHintButton = false;
	bool isInsideResetButton = false;
	bool autoRunMotion = false;
	bool autorun = false;
	bool check = false;
	bool win = false;
	const double glTabCoordXThousands[4] = { -0.68, -0.33, 0.02, 0.37 };
	const double glTabCoordXHundreds[4] = { -0.65, -0.30, 0.05, 0.4 };
	const double glTabCoordXTens[4] = { -0.6, -0.25, 0.1, 0.45 };
	const double glTabCoordXUnits[4] = { -0.58, -0.23, 0.12, 0.47 };
	const double glTabCoordY[4] = { 0.37, 0.02, -0.33, -0.68 };

	void specialInput(int key, int x, int y);
	void keyboard(unsigned char c, int x, int y);
	bool isInsideAutoButton(double x, double y);
	bool isInsideReset(double x, double y);
	void mouseMotion(int x, int y);
	void glInitialize(void);
	void writeText(t_Color text, double posX, double posY, double posZ, double sizeX, double sizeY, double sizeZ, std::string str);
	void numberToColor(unsigned int number, t_Color *block, t_Color *text);
	void mouse(int button, int state, int x, int y);
	void drawArrow(void);
	void render(void);

#endif
