#include "Menu.hpp"

using namespace ui;

Menu::Menu()
	: root{std::make_shared<SubMenu>(*this, "root")}
{
	history.push(root.get());
}

Menu::~Menu()
{}

Item& Menu::addItem(std::string&& label, const Item::ExecCallback& execCB)
{
	return root->addItem(std::move(label), execCB);
}

Item& Menu::addItem(std::string&& label, const Item::ExecCallback& execCB, const Item::UpdateCallback& updateCB)
{
	return root->addItem(std::move(label), execCB, updateCB);
}

SubMenu& Menu::addSubmenu(std::string&& label, const Item::ExecCallback& execCB)
{
	return root->addSubmenu(std::move(label), execCB);
}

SubMenu& Menu::addSubmenu(std::string&& label, const Item::ExecCallback& execCB, const Item::UpdateCallback& updateCB, const SubMenu::BackCallback& backCB)
{
	return root->addSubmenu(std::move(label), execCB, updateCB, backCB);
}

void Menu::registerToMenu(SubMenu& submenu)
{
	submenu.setExecCallback([&, cb = submenu.getExecCallback()]() 
	{
		history.push(&submenu); 
		cb(); 
	});

	submenu.setBackCallback([&, cb = submenu.getBackCallback()]()
	{
		cb(); 
		history.pop(); 
	});
}

void Menu::moveToCoords() const
{
	Component::moveToCoords();

	history.top()->printItems();
}

void Menu::addToHistory(SubMenu& submenu)
{
	history.push(&submenu);
}

void Menu::removeFromHistory()
{
	history.pop();
}

void Menu::execute() const
{
	history.top()->executeSelected();
}

void Menu::moveUp() const
{
	history.top()->moveUp();
}

void Menu::moveDown() const
{
	history.top()->moveDown();
}

void Menu::back() const
{
	if (history.size() == 1)
	{
		return;
	}

	history.top()->back();
}

size_t Menu::numberOfItems() const
{
	return history.top()->numberOfItems();
}

Item& Menu::getSelectedItem()
{
	return history.top()->getSelected();
}

SubMenu& Menu::getParentSubMenu()
{
	return *(history.top());
}

std::vector<SubMenu*> Menu::getParents()
{
	std::vector<SubMenu*> parents;
	std::copy(history._Get_container().begin(),
		history._Get_container().end(),
		std::back_inserter(parents));

	return parents;
}