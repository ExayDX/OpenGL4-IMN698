#ifndef _DEFAULT_TEST_LEVEL_H_
#define _DEFAULT_TEST_LEVEL_H_

# include "Scene.h"

class DefaultTestLevel : public Scene
{
public : 
	DefaultTestLevel();
	virtual void draw() override; 

private : 
	virtual void levelSetup() override;
	virtual void lightSetup() override;
	virtual void createShaderPrograms() override;
	virtual void createMaterials() override;

};

#endif // _DEFAULT_TEST_LEVEL_H_
