#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

class Player
{
private:
	string name;
	int level;
	int maxHp;
	int currentHp;
	int maxMp;
	int currentMp;
	int att;
	int def;
	int speed;

public:
	Player(string n, int lv, int mh, int ch, int mm, int cm, int a, int d, int s) : name(n), level(lv), maxHp(mh), currentHp(ch), maxMp(mm), currentMp(cm), att(a), def(d), speed(s) {} 

	int takeDamage(int damage)
	{
		int finalDamage = (damage - def / 2);

		if (finalDamage < 1)
		{
			finalDamage = 1; // 최소 데미지 1 보장
		}

		currentHp -= finalDamage;

		if (currentHp < 0)
		{
			currentHp = 0;
		}

		return finalDamage;
	}

	void PlayerDie()
	{
		cout << endl << getName() << "님이 쓰러졌습니다." << endl;
		cout << "Game Over" << endl;
	}

	string getName() { return name; }
	int getMaxHp() { return maxHp; }
	int getCurrentHp() { return currentHp; }
	int getAtt() { return att; }
	int getSpeed() { return speed; }
};

class Monster
{
private:
	string name;
	int hp;
	int mp;
	int att;
	int def;
	int speed;

public:
	Monster(string n, int h, int m, int a, int d, int s) : name(n), hp(h), mp(m), att(a), def(d), speed(s) {}

	int takeDamage(int damage)
	{
		int finalDamage = (damage - def / 2);

		if (finalDamage < 1)
		{
			finalDamage = 1;
		}

		hp -= finalDamage;

		if (hp < 0)
		{
			hp = 0;
		}

		return finalDamage;
	}

	void MonsterDie()
	{
		cout << endl << getName() << "을 쓰러뜨렸습니다." << endl;
	}

	string getName() { return name; }
	int getHp() { return hp; }
	int getAtt() { return att; }
	int getSpeed() { return speed; }
	
};

class Slime : public Monster
{
public:
	Slime() : Monster("슬라임", 20, 10, 5, 5, 3) {}
};

class Ork : public Monster
{
public:
	Ork() : Monster("오크", 30, 5, 1000, 5, 2) {}
};

class Wolf : public Monster
{
public:
	Wolf() : Monster("늑대", 50, 20, 30, 10, 15) {}

};

class Skill
{
protected:
	string name;
	int mpConsume;

public: 
	Skill(string n, int m) : name(n), mpConsume(m){}

	virtual ~Skill() {}

	virtual void useSkill(Player& p, Monster& m) = 0;

	string getName() { return name; }
	int getMpConsume() { return mpConsume; }

};

class Fireball : public Skill
{
public:
	Fireball() : Skill("파이어볼", 20) {}

	void useSkill(Player& p, Monster& m) override
	{
		{
			cout << endl << getName() << "을 사용합니다" << endl;
			int damage = p.getAtt() + 20;
			int finalDmg = m.takeDamage(damage);
		}

	}
};

class Event
{
public:
	virtual void execute(Player& player) = 0;
};

class BattleEvent : public Event
{
public:
	void execute(Player& player) override
	{
		Monster* monster = nullptr;
		int randomValue = rand() % 100;

		if (randomValue < 50) monster = new Slime();
		else if (randomValue < 80) monster = new Wolf();
		else monster = new Ork();

		if (monster != nullptr) {
			cout << "\n야생의 [" << monster->getName() << "]이(가) 나타났습니다!" << endl;
			battle(player, *monster);
			delete monster;
		}
	}

	bool playerBattle(Player& p, Monster& m, int turn)
	{
		cout << "-" << turn << "번째 " << p.getName() << "의 턴-" << endl;
		int mDmg = m.takeDamage(p.getAtt());
		cout << m.getName() << "에게 " << mDmg << "의 피해를 입혔습니다." << endl;
		cout << m.getName() << " HP: " << m.getHp() << endl;

		if (m.getHp() <= 0) {
			m.MonsterDie();
			return true; 
		}
		return false;
	}

	bool monsterBattle(Player& p, Monster& m, int turn)
	{
		cout << "-" << turn << "번째 " << m.getName() << "의 턴 -" << endl;
		int pDmg = p.takeDamage(m.getAtt());
		cout << p.getName() << "는 " << pDmg << "의 피해를 입었습니다." << endl;

		cout << p.getName() << " HP: " << p.getCurrentHp() << " / " << p.getMaxHp() << endl;

		if (p.getCurrentHp() <= 0) {
			p.PlayerDie();
			return true; 
		}
		return false;
	}

	bool battleMessage = true;

	bool run(Player& p, Monster& m)
	{
		int battleRun;

		if (battleMessage == true)
		{
			cout << "도망가시겠습니까? 1.예 2.아니오";
			cin >> battleRun;

			if (battleRun == 1)
			{
				if (p.getSpeed() > m.getSpeed()) // 플레이어 속도가 빠르면 성공
				{
					cout << "전투를 빠져나옵니다." << endl;
					cin.get();
					return true;
				}
				else
				{
					cout << m.getName() << "에게 따라잡힙니다."<< endl;
					battleMessage = false;
					cin.get();
					return false;
				}
			}
			else if (battleRun == 2)
			{
				cout << "전투를 계속합니다." << endl;
				battleMessage = false;
				cin.get();
				return false;
			}
			else
			{
				cout << "잘못된 입력입니다." << endl;
				return false;
			}

		}
	}

	void battle(Player& p, Monster& m)
	{
		int turn = 1;
		cout << m.getName() << "와(과)의 전투를 시작합니다..." << endl << endl;

		while (p.getCurrentHp() > 0 && m.getHp() > 0)
		{

			if (run(p, m))
			{
				break;
			}

			if (p.getSpeed() >= m.getSpeed()) // 플레이어 선공
			{
				if (playerBattle(p, m, turn)) break;
				cin.get();
				if (monsterBattle(p, m, turn)) break;
				cin.get();
			}
			else // 몬스터 선공
			{
				if (monsterBattle(p, m, turn)) break;
				cin.get();
				if (playerBattle(p, m, turn)) break;
				cin.get();
			}

			turn++;

		}
	}
};
int main()
{
	srand((unsigned int)time(NULL));
	string nickname;
	int menuChoice;

	cout << "당신의 이름을 입력하시오.: ";
	cin >> nickname;

	Player player(nickname, 1, 100, 100, 100, 100, 20, 20, 10);
	cout << nickname << "님 환영합니다." << endl;

	while (true)
	{
		cout << "\n┌─────────────┐" << endl;
		cout << "│     메뉴    │" << endl;
		cout << "├─────────────┤" << endl;
		cout << "│ 1. 모험하기 │" << endl;
		cout << "│ 0. 종료     │" << endl;
		cout << "└─────────────┘" << endl;
		cout << "당신의 행동: ";

		if (!(cin >> menuChoice)) {
			cin.clear();
			cin.ignore(100, '\n');
			continue;
		}
		cin.ignore();

		switch (menuChoice)
		{
		case 1:
		{
			BattleEvent bEvent;
			bEvent.execute(player);
			if (player.getCurrentHp() <= 0) return 0;
		}
		break;
		case 0:
			cout << "게임을 종료합니다." << endl;
			return 0;
		default:
			cout << "잘못된 입력입니다." << endl;
			break;
		}
	}
	return 0;
}