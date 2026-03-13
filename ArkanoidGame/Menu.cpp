#include "Menu.h"
#include <assert.h>
#include <algorithm>

namespace ArkanoidGame
{
	// Helper: get node by index path
	MenuItem* Menu::getNodeByPath(const std::vector<std::size_t>& path)
	{
		MenuItem* node = &rootItem;
		for (size_t idx : path) {
			if (idx >= node->childrens.size()) return nullptr;
			node = &node->childrens[idx];
		}
		return node;
	}

	bool Menu::findPathToItem(MenuItem* current, MenuItem* target, std::vector<std::size_t>& outPath)
	{
		if (current == target) return true;
		for (size_t i = 0; i < current->childrens.size(); ++i) {
			outPath.push_back(i);
			if (findPathToItem(&current->childrens[i], target, outPath)) return true;
			outPath.pop_back();
		}
		return false;
	}

	void Menu::Init(const MenuItem& item)
	{
		rootItem = item;

		InitMenuItem(rootItem);
		if (!rootItem.childrens.empty()) {
			selectionPath.clear();
			selectionPath.push_back(0); // select first child of root
			MenuItem* sel = getNodeByPath(selectionPath);
			if (sel) sel->text.setFillColor(sel->selectedColor);
		}
	}
	
	void Menu::InitMenuItem(MenuItem& item)
	{
		for (auto& child : item.childrens)
		{
			child.parent = &item;
			InitMenuItem(child);
		}
	}

	void Menu::Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin)
	{
		MenuItem* expanded = nullptr;
		if (!selectionPath.empty()) {
			std::vector<std::size_t> contextPath = selectionPath;
			contextPath.pop_back(); // expanded is parent of selected
			expanded = getNodeByPath(contextPath);
		}
		if (!expanded) expanded = &rootItem;

		std::vector<sf::Text*> texts;
		texts.reserve(expanded->childrens.size());
		for (auto& child : expanded->childrens) {
			if (child.isEnabled) {
				texts.push_back(&child.text);
			}
		}

		DrawTextList(
			window,
			texts,
			expanded->childrenSpacing,
			expanded->childrenOrientation,
			expanded->childrenAlignment,
			position,
			origin);
	}
	
	MenuItem* getSelectedItemHelper(MenuItem* root, const std::vector<std::size_t>& path)
	{
		MenuItem* node = root;
		for (size_t i = 0; i < path.size(); ++i) {
			std::size_t idx = path[i];
			if (idx >= node->childrens.size()) return nullptr;
			node = &node->childrens[idx];
		}
		return node;
	}

	void Menu::PressOnSelectedItem()
	{
		if (selectionPath.empty()) return;
		MenuItem* selected = getNodeByPath(selectionPath);
		if (!selected) return;

		if (selected->onPressCallback) {
			selected->onPressCallback(*selected);
			return;
		}

		if (!selected->childrens.empty()) {
			// enter submenu: append 0 to path and select first child
			selectionPath.push_back(0);
			MenuItem* newSel = getNodeByPath(selectionPath);
			if (newSel) newSel->text.setFillColor(newSel->selectedColor);
		}
	}

	void Menu::GoBack()
	{
		if (selectionPath.empty()) return;
		// if at root child, collapse to root (clear path)
		if (selectionPath.size() == 1) {
			// deselect current
			MenuItem* cur = getNodeByPath(selectionPath);
			if (cur) cur->text.setFillColor(cur->deselectedColor);
			selectionPath.clear();
			return;
		}
		// deselect current
		MenuItem* cur = getNodeByPath(selectionPath);
		if (cur) cur->text.setFillColor(cur->deselectedColor);
		// pop last index - move to parent and select that parent item index
		selectionPath.pop_back();
		// ensure selected index exists
		MenuItem* newSel = getNodeByPath(selectionPath);
		if (newSel) newSel->text.setFillColor(newSel->selectedColor);
	}

	void Menu::SwitchToPreviousMenuItem()
	{
		if (selectionPath.empty()) return;
		MenuItem* selected = getNodeByPath(selectionPath);
		if (!selected || !selected->parent) return;
		MenuItem* parent = selected->parent;
		std::size_t idx = selectionPath.back();
		if (idx > 0) {
			// deselect current
			selected->text.setFillColor(selected->deselectedColor);
			selectionPath.back() = idx - 1;
			MenuItem* newSel = getNodeByPath(selectionPath);
			if (newSel) newSel->text.setFillColor(newSel->selectedColor);
		}
	}

	void Menu::SwitchToNextMenuItem()
	{
		if (selectionPath.empty()) return;
		MenuItem* selected = getNodeByPath(selectionPath);
		if (!selected || !selected->parent) return;
		MenuItem* parent = selected->parent;
		std::size_t idx = selectionPath.back();
		if (idx + 1 < parent->childrens.size()) {
			// deselect current
			selected->text.setFillColor(selected->deselectedColor);
			selectionPath.back() = idx + 1;
			MenuItem* newSel = getNodeByPath(selectionPath);
			if (newSel) newSel->text.setFillColor(newSel->selectedColor);
		}
	}

	void Menu::SelectMenuItem(MenuItem& item)
	{
		// find path to item from root
		std::vector<std::size_t> path;
		if (!findPathToItem(&rootItem, &item, path)) return;

		// deselect previous
		if (!selectionPath.empty()) {
			MenuItem* prev = getNodeByPath(selectionPath);
			if (prev) prev->text.setFillColor(prev->deselectedColor);
		}

		selectionPath = path;
		MenuItem* newSel = getNodeByPath(selectionPath);
		if (newSel) newSel->text.setFillColor(newSel->selectedColor);
	}

	MenuItem& Menu::GetCurrentContext()
	{
		if (selectionPath.empty()) return rootItem;
		// parent of selected item is context
		std::vector<std::size_t> contextPath = selectionPath;
		contextPath.pop_back();
		MenuItem* ctx = getNodeByPath(contextPath);
		if (!ctx) return rootItem;
		return *ctx;
	}
}
