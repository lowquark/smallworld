#ifndef SPRITE3D_H
#define SPRITE3D_H

#include <iostream>

#include <DL/DEngine.h>

class Sprite3D : public dl::Renderable
{
  public:

	enum Orientation { LEFT_FRONT, LEFT_BACK, RIGHT_FRONT, RIGHT_BACK };

  private:

	Orientation m_orientation;

	bool m_stepping;

  public:

	Sprite3D();

	bool isStepping() const;
	void setStepping(bool _stepping);

	Orientation getOrientation() const;
	void setOrientation(Orientation _orientation);

	void render() const;
};

#endif
