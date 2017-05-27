#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <string>
#include <cmath>
#include <unistd.h>
#include <GL/glut.h>
#include "game.h"
#include "ai.h"
#include "gui.h"

unsigned int **playGrid;
unsigned int **copyGrid;
unsigned int scoreTotal;

e_Direction bDir = NONE; //A mettre dans un .c

void specialInput(int key, int x, int y)
{
	if (autorun)
		return;
	e_Direction dir;
	bool b1;
	unsigned int add, nbSlide;
	switch (key)
	{
		case GLUT_KEY_UP:
			dir = UP;
			break;
		case GLUT_KEY_DOWN:
			dir = DOWN;
			break;
		case GLUT_KEY_LEFT:
			dir = LEFT;
			break;
		case GLUT_KEY_RIGHT:
			dir = RIGHT;
			break;
		default:
			return;
	}
	b1 = slide_space(dir, playGrid);
	nbSlide = slide_couple(dir, playGrid, &add);
	scoreTotal += add;
	if (b1 || nbSlide != 0)
		place_2or4(playGrid);
	copyBoard(playGrid, copyGrid);
	if (!isPlayable(copyGrid))
		return;
	bDir = NONE;
	glutPostRedisplay();

}
void keyboard(unsigned char c, int x, int y)
{
	if (c == 27)
		autorun = false;
}

bool isInsideAutoButton(double x, double y)
{
	return (x >= 0.75 && x <= 0.96 && y >= -0.8 && y <= -0.7); 
}

bool isInsideReset(double x, double y)
{
	return (x <= -0.75 && x >= -0.96 && y >= -0.8 && y <= -0.7); 
}

void mouseMotion(int x, int y)
{
	double normalizedX = (double)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
	double normalizedY = -((double)y / glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1);
	double distancesquared = (normalizedX - 0.85) * (normalizedX - 0.85) + (normalizedY - 0.5) * (normalizedY - 0.5);

	//mouse inside hint button
	if (distancesquared <= 0.01)
		isInsideHintButton = true;
	else
		isInsideHintButton = false;
	autoRunMotion = isInsideAutoButton(normalizedX, normalizedY);
	isInsideResetButton = isInsideReset(normalizedX, normalizedY);
	glutPostRedisplay();
}


void resize(int w, int h)
{
	WIDTH = glutGet(GLUT_WINDOW_WIDTH);
	HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, 0, HEIGHT, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void glInitialize(void)
{
	glClearColor(250.0/255.0f, 248.0/255.0f, 239.0f/255.0, 0.0f);
}


void writeText(t_Color text, double posX, double posY, double posZ, double sizeX, double sizeY, double sizeZ, std::string str)
{
	glPushMatrix();
	glColor3d(text.r, text.g, text.b);
	glTranslatef(posX, posY, posZ);
	glScalef(sizeX, sizeY, sizeZ);
	for (size_t i = 0; i < str.length(); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	glPopMatrix();
}

void numberToColor(unsigned int number, t_Color *block, t_Color *text)
{
	switch (number)
	{
		case 2:
			block->r = 238.0f / 255.0f;
			block->g = 228.0f / 255.0f;
			block->b = 218.0f / 255.0f;
			text->r = 0.0f;
			text->g = 0.0f;
			text->b = 0.0f;
			break;
		case 4:
			block->r = 237.0f / 255.0f;
			block->g = 224.0f / 255.0f;
			block->b = 200.0f / 255.0f;
			text->r = 0.0f;
			text->g = 0.0f;
			text->b = 0.0f;
			break;
		case 8:
			block->r = 242.0f / 255.0f;
			block->g = 177.0f / 255.0f;
			block->b = 121.0f / 255.0f;
			text->r = 1.0f;
			text->g = 1.0f;
			text->b = 1.0f;
			break;
		case 16:
			block->r = 245.0f / 255.0f;
			block->g = 149.0f / 255.0f;
			block->b = 99.0f / 255.0f;
			text->r = 1.0f;
			text->g = 1.0f;
			text->b = 1.0f;
			break;
		case 32:
			block->r = 246.0f / 255.0f;
			block->g = 124.0f / 255.0f;
			block->b = 95.0f / 255.0f;
			text->r = 1.0f;
			text->g = 1.0f;
			text->b = 1.0f;
			break;
		case 64:
			block->r = 246.0f / 255.0f;
			block->g = 94.0f / 255.0f;
			block->b = 59.0f / 255.0f;
			text->r = 1.0f;
			text->g = 1.0f;
			text->b = 1.0f;
			break;
		case 128:
			block->r = 237.0f / 255.0f;
			block->g = 207.0f / 255.0f;
			block->b = 114.0f / 255.0f;
			text->r = 1.0f;
			text->g = 1.0f;
			text->b = 1.0f;
			break;
		case 256:
			block->r = 237.0f / 255.0f;
			block->g = 204.0f / 255.0f;
			block->b = 97.0f / 255.0f;
			text->r = 1.0f;
			text->g = 1.0f;
			text->b = 1.0f;
			break;
		case 512:
			block->r = 237.0f / 255.0f;
			block->g = 200.0f / 255.0f;
			block->b = 80.0f / 255.0f;
			text->r = 1.0f;
			text->g = 1.0f;
			text->b = 1.0f;
			break;
		case 1024:
			block->r = 237.0f / 255.0f;
			block->g = 197.0f / 255.0f;
			block->b = 63.0f / 255.0f;
			text->r = 1.0f;
			text->g = 1.0f;
			text->b = 1.0f;
			break;
		case 2048:
			block->r = 237.0f / 255.0f;
			block->g = 194.0f / 255.0f;
			block->b = 46.0f / 255.0f;
			text->r = 1.0f;
			text->g = 1.0f;
			text->b = 1.0f;
			break;
		default:
			block->r = 0.0f;
			block->g = 0.0f;
			block->b = 0.0f;
			text->r = 1.0f;
			text->g = 1.0f;
			text->b = 1.0f;
			break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if (autoRunMotion && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		//	unsigned int i = 0;
		//	stopAutoRun = false;
			/*while (!stopAutoRun && isPlayable(copyGrid) && i != 5)
			{
			*/
			playAI();
			autorun = true;
			
		//	i++;
		//}
	}
	else if (isInsideHintButton && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		bDir = getBestDirection(playGrid, dep[countBlanks(playGrid)]);
	else if (button == GLUT_RIGHT_BUTTON)
		autorun = false;
	else if (isInsideResetButton && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		scoreTotal = 0;
		win = false;
		fillZeros(playGrid);
		initPlays(playGrid);
	}
	glutPostRedisplay();
}


void drawArrow(void)
{
	glColor3f(0.0f, 0.0f, 0.0f);
	switch (bDir)
	{
		case UP:
			glBegin(GL_LINE_LOOP);
				glVertex2f(0.85, 0.35);
				glVertex2f(0.90, 0.32);
				glVertex2f(0.87, 0.32);
				glVertex2f(0.87, 0.29);
				glVertex2f(0.83, 0.29);
				glVertex2f(0.83, 0.32);
				glVertex2f(0.80, 0.32);
			glEnd();
			break;
		case DOWN:
			glBegin(GL_LINE_LOOP);
				glVertex2f(0.83, 0.35);
				glVertex2f(0.87, 0.35);
				glVertex2f(0.87, 0.32);
				glVertex2f(0.90, 0.32);
				glVertex2f(0.85, 0.29);
				glVertex2f(0.80, 0.32);
				glVertex2f(0.83, 0.32);
			glEnd();
			break;
		case RIGHT:
			glBegin(GL_LINE_LOOP);
				glVertex2f(0.82, 0.31);
				glVertex2f(0.85, 0.31);
				glVertex2f(0.85, 0.34);
				glVertex2f(0.88, 0.29);
				glVertex2f(0.85, 0.24);
				glVertex2f(0.85, 0.27);
				glVertex2f(0.82, 0.27);
			glEnd();
			break;
		case LEFT:
			glBegin(GL_LINE_LOOP);
				glVertex2f(0.82, 0.30);
				glVertex2f(0.85, 0.35);
				glVertex2f(0.85, 0.32);
				glVertex2f(0.88, 0.32);
				glVertex2f(0.88, 0.28);
				glVertex2f(0.85, 0.28);
				glVertex2f(0.85, 0.25);
			glEnd();
			break;
		default:
			break;
	}
}

void render(void)
{
	t_Color block;
	t_Color titleText;
	t_Color text;

	titleText.r = titleText.g = titleText.b = 0.0f;
	text.r = text.g = text.b = 1.0f;
	glInitialize();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
		glColor3f(205.0 / 255.0f, 193.0 / 255.0f, 180.0 / 255.0f);
		glVertex2f(-0.7, 0.6);
		glVertex2f(0.7, 0.6);
		glVertex2f(0.7, -0.8);
		glVertex2f(-0.7, -0.8);
	glEnd();
	for (int j = 0; j < MAX_V; j++)
	{
		for (int i = 0; i < MAX_H; i++)
		{
			if (playGrid[j][i] != 0)
			{
				numberToColor(playGrid[j][i], &block, &text);
				glBegin(GL_QUADS);
					glColor3f(block.r, block.g, block.b);
					glVertex2f(-0.7 + 0.35 * i, 0.6 - 0.35 * j);
					glVertex2f(-0.7 + 0.35 * (i + 1), 0.6 - 0.35 * j);
					glVertex2f(-0.7 + 0.35 * (i + 1), 0.6 - 0.35 * (j + 1));
					glVertex2f(-0.7 + 0.35 * i, 0.6 - 0.35 * (j + 1));
				glEnd();
				if (playGrid[j][i] > 1000)
					writeText(text, glTabCoordXThousands[i], glTabCoordY[j], 0.0, 0.0010, 0.0010, 0.0, std::to_string(playGrid[j][i]));
				else if (playGrid[j][i] > 100)
					writeText(text, glTabCoordXHundreds[i], glTabCoordY[j], 0.0, 0.0010, 0.0010, 0.0, std::to_string(playGrid[j][i]));
				else if (playGrid[j][i] > 10)
					writeText(text, glTabCoordXTens[i], glTabCoordY[j], 0.0, 0.0010, 0.0010, 0.0, std::to_string(playGrid[j][i]));
				else
					writeText(text, glTabCoordXUnits[i], glTabCoordY[j], 0.0, 0.0010, 0.0010, 0.0, std::to_string(playGrid[j][i]));
			}
		}
	}
	glBegin(GL_QUADS);
		glColor3f(187.0 / 255.0f, 173.0 / 255.0f, 160.0 / 255.0f);
		for (double index = -0.7; index < 0.71; index = index + 0.35)
		{
			if (index != -0.7 && index != 0.7)
			{
				glVertex2f(index - 0.02, 0.6);
				glVertex2f(index + 0.02, 0.6);
				glVertex2f(index + 0.02, -0.8);
				glVertex2f(index - 0.02, -0.8);
			}
			else if (index == 0.7)
			{
				glVertex2f(index - 0.02, 0.6);
				glVertex2f(index, 0.6);
				glVertex2f(index, -0.8);
				glVertex2f(index - 0.02, -0.8);
			}
			else
			{
				glVertex2f(index, 0.6);
				glVertex2f(index + 0.02, 0.6);
				glVertex2f(index + 0.02, -0.8);
				glVertex2f(index, -0.8);
			}
		}
		for (double index = 0.6; index > -0.8; index = index - 0.35)
		{
			glVertex2f(-0.7, index - 0.02);
			glVertex2f(-0.7, index + 0.02);
			glVertex2f(0.7, index + 0.02);
			glVertex2f(0.7, index - 0.02);
		}
	glEnd();
	for (double index = -0.85; index <= -0.84; index += 0.001)
		writeText(titleText, index, 0.7, 0.0, 0.0015, 0.0015, 0.0, title);
	writeText(titleText, 0.0, 0.7, 0.0, 0.0008, 0.0008, 0.0, scoreTitle);
	writeText(titleText, 0.35, 0.7, 0.0, 0.0008, 0.0006, 0.0, std::to_string(scoreTotal));
	/*buttons*/
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.0f, 75.0 / 255.0f, 141.0 / 255.0f);
		glVertex2f(0.85, 0.5);
		for (unsigned int i = 0; i <= 100; i++)
		{
			glVertex2f(
				(0.85 + (0.1 * cos(i * 2 * M_PI / 100))), (0.5 + (0.1 * sin(i * 2 * M_PI / 100)))
			);
		}
	glEnd();
	t_Color hintAndAuto;
	hintAndAuto.r = hintAndAuto.g = hintAndAuto.b = 1.0f;
	for (double indexj = 0.469; indexj <= 0.471; indexj += 0.001)
	{
		for (double indexi = 0.816; indexi <= 0.824; indexi += 0.001)
			writeText(hintAndAuto, indexi, indexj, 0.0, 0.0008, 0.0006, 0.0, "?");
	}
	if (isInsideHintButton)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		for (double index = 0.09; index <= 0.1; index += 0.001)
		{
			glBegin(GL_LINE_LOOP);
				for (unsigned int i = 0; i <= 30; i++)
				{
					glVertex2f(
						(0.85 + (index * cos(i * 2 * M_PI / 30))), (0.5 + (index * sin(i * 2 * M_PI / 30)))
					);
				}
			glEnd();
		}
	}
	drawArrow();
	glColor3f(0.0f, 75.0 / 255.0f, 141.0 / 255.0f);
	glBegin(GL_QUADS);
		glVertex2f(0.75, -0.70);
		glVertex2f(0.96, -0.70);
		glVertex2f(0.96, -0.80);
		glVertex2f(0.75, -0.80);
	glEnd();
	glColor3f(0.0f, 75.0 / 255.0f, 141.0 / 255.0f);
	glBegin(GL_QUADS);
		glVertex2f(-0.75, -0.70);
		glVertex2f(-0.96, -0.70);
		glVertex2f(-0.96, -0.80);
		glVertex2f(-0.75, -0.80);
	glEnd();
	if (isInsideResetButton)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		for (double index = 0.000; index <= 0.006; index += 0.001)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f(-0.75 + index, -0.70 + index);
				glVertex2f(-0.96 - index, -0.70 + index);
				glVertex2f(-0.96 - index, -0.80 - index);
				glVertex2f(-0.75 + index, -0.80 - index);
			glEnd();
		}
	}
	if (autoRunMotion)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		for (double index = 0.000; index <= 0.006; index += 0.001)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f(0.75 - index, -0.70 + index);
				glVertex2f(0.96 + index, -0.70 + index);
				glVertex2f(0.96 + index, -0.80 - index);
				glVertex2f(0.75 - index, -0.80 - index);
			glEnd();
		}
	}
	writeText(hintAndAuto, -0.93, -0.77, 0.0, 0.00042, 0.00042, 0.0, "Reset");
	writeText(hintAndAuto, 0.75, -0.77, 0.0, 0.00042, 0.00042, 0.0, "AutoRun");
	if (!win)
	{
		if (searchBestValue(playGrid) >= 2048)
			win = true;
	}
	if (win)
		writeText(titleText, -0.1, -0.95, 0.0, 0.00042, 0.00042, 0.0, "YOU WON");
	if (autorun)
	{
		if (!playAI())
		{
			autorun = false;
		}
	}
	glutSwapBuffers();
}

int main (int argc, char **argv)
{
	srand(time(NULL));
	playGrid = init_Grid();
	copyGrid = init_Grid();
	initPlays(playGrid);
	scoreTotal = 0;
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("2048");

	glutSpecialFunc(specialInput);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouseMotion);
	glutMouseFunc(mouse);
	glutDisplayFunc(render);
	glutPostRedisplay();
//	glutIdleFunc(render);
	glutMainLoop();
	return (1);
}
