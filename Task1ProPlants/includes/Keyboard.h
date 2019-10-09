#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
		Event() noexcept : _type(Type::Invalid), _code(0u) { }
		Event(Type type, unsigned char code) noexcept : _type(type), _code(code) { }

		inline bool IsPress() const noexcept				{ return _type == Type::Press; }
		inline bool IsRelease() const noexcept				{ return _type == Type::Release; }
		inline bool IsValid() const noexcept				{ return _type != Type::Invalid; }
		inline unsigned char GetCode() const noexcept		{ return _code; }
	private:
		Type _type;
		unsigned char _code;
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	//Key event stuff
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;

	//Char event stuff
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;

	//Affects both queues
	void Flush() noexcept;

	//autorepeat control
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() const noexcept;

private:
	//These functions are used by window the friend class, client aka our game cant use these
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
	static constexpr unsigned int _keys = 256u;
	static constexpr unsigned int _bufferSize = 16u;
	bool _autorepeatEnabled = false;
	std::bitset<_keys> _keyStates;
	std::queue<Event> _keyBuffer;
	std::queue<char> _charBuffer;
};
