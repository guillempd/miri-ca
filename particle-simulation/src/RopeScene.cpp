#include "RopeScene.h"

#include <imgui.h>

RopeScene::RopeScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh)
	: Scene(materials, planeMesh)
{
}

void RopeScene::Update(float dt)
{
	CreateInterface();
}

void RopeScene::SetupEntities()
{

}

void RopeScene::CreateInterface()
{
	if (ImGui::Begin("Welcome to the Rope Scene"))
	{
		ImGui::Text("WELCOME!");
	}
	ImGui::End();
}
