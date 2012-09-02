#include "Sprite3D.h"

using namespace std;

Sprite3D::Sprite3D() : m_orientation(LEFT_FRONT), m_stepping(false)
{
}

bool Sprite3D::isStepping() const
{
	return m_stepping;
}
void Sprite3D::setStepping(bool _stepping)
{
	m_stepping = _stepping;
}

Sprite3D::Orientation Sprite3D::getOrientation() const
{
	return m_orientation;
}
void Sprite3D::setOrientation(Orientation _orientation)
{
	m_orientation = _orientation;
}

void Sprite3D::render() const
{
	float xMin;
	float xMax;
	float yMin;
	float yMax;

	switch(m_orientation)
	{
		case LEFT_FRONT:
			xMin = 0.0 + .001;
			xMax = 0.5 - .001;
			break;
		case LEFT_BACK:
			xMin = 0.5 + .001;
			xMax = 1.0 - .001;
			break;
		case RIGHT_FRONT:
			xMin = 0.5 + .001;
			xMax = 0.0 - .001;
			break;
		case RIGHT_BACK:
			xMin = 1.0 + .001;
			xMax = 0.5 - .001;
			break;
	}

	if(m_stepping)
	{
		yMin = 0.5 + .001;
		yMax = 1.0 - .001;
	}
	else
	{
		yMin = 0 + .001;
		yMax = .5 - .001;
	}

	glBegin(GL_QUADS);

		glTexCoord2d(xMax, yMax); glVertex3f(0.5,  0.0, 0.0);
		glTexCoord2d(xMax, yMin); glVertex3f(0.5,  0.0, 1.0);
		glTexCoord2d(xMin, yMin); glVertex3f(-0.5, 0.0, 1.0);
		glTexCoord2d(xMin, yMax); glVertex3f(-0.5, 0.0, 0.0);

		glColor3f(0.0, 0.0, 0.0);

		for(float i = .01;i <= .1;i += .01)
		{
			glTexCoord2d(xMax, yMax); glVertex3f(0.5,  i, 0.0);
			glTexCoord2d(xMax, yMin); glVertex3f(0.5,  i, 1.0);
			glTexCoord2d(xMin, yMin); glVertex3f(-0.5, i, 1.0);
			glTexCoord2d(xMin, yMax); glVertex3f(-0.5, i, 0.0);
		}

	glEnd();
}
