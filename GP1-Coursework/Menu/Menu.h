#pragma once

#include "../Engine/GameContext.h"
#include "../Engine/Console.h"
#include "../Games/IGame.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

class Menu {
public:
	using Factory = std::function<std::unique_ptr<IGame>(GameContext&)>;

	void Add(std::wstring name, Factory factory);

	std::unique_ptr<IGame> Update(GameContext& context);
	void Draw(Console& console) const;
	bool WantsToQuit() const { return m_wantsToQuit; }

private:
	struct Entry {
		std::wstring name;
		Factory createFactory;
	};
	std::vector<Entry> m_entries;
	int m_selectedIndex = 0;
	bool m_wantsToQuit = false;
};