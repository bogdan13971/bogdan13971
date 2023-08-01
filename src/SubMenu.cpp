#include "SubMenu.hpp"
#include "ConsoleCommands.hpp"
#include "Menu.hpp"

using namespace ui;

SubMenu::SubMenu(Menu& menu,std::string&& label, const ExecCallback& execCB)
	: SubMenu{ menu,std::move(label), execCB, [&]() {return label;}, NO_OP_CB }
{}

SubMenu::SubMenu(Menu& menu,std::string&& label,
	const ExecCallback& execCB,
	const UpdateCallback& updateCB,
	const BackCallback& backCB)
	: Item{ std::move(label), execCB, updateCB },
	backCallback{ backCB },
	selected{ 0 },
	menu{menu}
{};

void SubMenu::setBackCallback(const BackCallback& backCB)
{
	backCallback = backCB;
}

const SubMenu::BackCallback& SubMenu::getBackCallback() const
{
	return backCallback;
}

void SubMenu::execute()
{
	Item::execute();

	menu.addToHistory(*this);

	std::cout << CLEAR_SCREEN;
}

void SubMenu::update()
{
	Item::update();

	for (const auto& item : items)
	{
		item->update();
	}
}

void SubMenu::printItems() const
{
	for (size_t i = 0; i < items.size(); i++)
	{
		std::cout << CLEAR_LINE;

		if (i == selected)
		{
			std::cout << SET_FORMAT;
		}

		items[i]->print();

		if (i == selected)
		{
			std::cout << CLEAR_FORMAT;
		}
	}

	std::cout << "\n";
}

void SubMenu::executeSelected() const
{
	if (items.empty())
	{
		return;
	}

	items[selected]->execute();
}

void SubMenu::moveUp() const
{
	if (selected == 0)
	{
		return;
	}

	selected--;
}

void SubMenu::moveDown() const
{
	if (selected == items.size() - 1)
	{
		return;
	}

	selected++;
}

void SubMenu::back() const
{
	backCallback();

	selected = 0;
	std::cout << CLEAR_SCREEN;

	menu.removeFromHistory();
}

Item& SubMenu::addItem(std::string&& label, const ExecCallback& execCB)
{
	items.push_back(std::make_unique<Item>(std::move(label), execCB));
	return *(items.back().get());
}

Item& SubMenu::addItem(std::string&& label, const ExecCallback& execCB, const UpdateCallback& updateCB)
{
	auto& item = addItem(std::move(label), execCB);
	item.setUpdateCallback(updateCB);
	return item;
}

SubMenu& SubMenu::addSubmenu(std::string&& label, const ExecCallback& execCB)
{
	items.push_back(std::make_unique<SubMenu>(menu, std::move(label), execCB));
	return dynamic_cast<SubMenu&>(*(items.back().get()));
}

SubMenu& SubMenu::addSubmenu(std::string&& label, const ExecCallback& execCB, const UpdateCallback& updateCB, const SubMenu::BackCallback& backCB)
{
	auto& submenu = addSubmenu(std::move(label), execCB);
	submenu.setUpdateCallback(updateCB);
	submenu.setBackCallback(backCB);
	return submenu;
}

Item& SubMenu::getSelected()
{
	return *(items[selected]);
}

std::vector<std::reference_wrapper<Item>> SubMenu::getItems()
{
	decltype(getItems()) result;
	for (const auto& item : items)
	{
		result.push_back(*item);
	}

	return result;
}

size_t SubMenu::numberOfItems() const
{
	return items.size();
}

const Item::ExecCallback Item::NO_OP_CB = []() {};