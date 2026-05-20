#pragma once
#include "SFML/Graphics.hpp"
#include "Text.h"
#include <list>
#include <functional>


namespace SnakeGame
{
	struct MenuItem
	{
		std::unique_ptr<sf::Text> text;
		std::unique_ptr<sf::Text> hintText;
		
		MenuItem()
		{
			text = std::make_unique<sf::Text>();
			hintText = std::make_unique<sf::Text>();
		}

		MenuItem(const MenuItem&) = delete;
		MenuItem& operator=(const MenuItem&) = delete;

		MenuItem(MenuItem&&) = delete;
		MenuItem& operator=(MenuItem&&) = delete;

		Orientation childrenOrientation = Orientation::Vertical;
		Alignment childrenAlignment = Alignment::Min;
		float childrenSpacing = 0.f;

		sf::Color selectedColor = sf::Color::Yellow;
		sf::Color deselectedColor = sf::Color::White;

		bool isEnabled = true;
		std::vector<std::unique_ptr<MenuItem>> childrens;

		std::function<void(MenuItem* item, Menu* menu)> onPressCallback; 

		MenuItem* parent = nullptr;

		void RefreshFonts(const sf::Font* newFont = nullptr)
		{
			if (newFont != nullptr)
			{
				text->setFont(*newFont);
				hintText->setFont(*newFont);
			}
			
			for (auto& child : childrens)
			{
				child->RefreshFonts(newFont);
			}
		}

		~MenuItem()
		{
			onPressCallback = nullptr;
			childrens.clear();
			text.reset();
			hintText.reset();
			parent = nullptr;
		}
	};

	class Menu
	{
	public:
		Menu() = default;

		Menu(const Menu&) = delete;
		Menu& operator=(const Menu&) = delete;

		Menu(Menu&&) = delete;
		Menu& operator=(Menu&&) = delete;

		void Init(const MenuItem& item, const sf::Font& menuFont);

		void Update(float deltaTime);

		void Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);

		void PressOnSelectedItem();	
		void GoBack();	
		void SwitchToPreviousMenuItem();
		void SwitchToNextMenuItem();

		MenuItem& GetCurrentContext();

	private:
		void InitMenuItem(MenuItem& item);
		void SelectMenuItem(MenuItem& item);

	private:
		std::unique_ptr<MenuItem> rootItem;
		MenuItem* selectedItem = nullptr;

		const sf::Font* font = nullptr;

	private:
		void SetupMenuItemFromSource(MenuItem& dest, const MenuItem& src, const sf::Font& font);
	};
}
