#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

class Skill;

class Player
{
private:
	string name;
	int level;
	int exp;
	int maxHp;
	int currentHp;
	int maxMp;
	int currentMp;
	int att;
	int def;
	int speed;
	vector<Skill*> skillList;

public:
	Player(string n, int lv, int ex, int mh, int ch, int mm, int cm, int a, int d, int s) : name(n), level(lv), exp(ex), maxHp(mh), currentHp(ch), maxMp(mm), currentMp(cm), att(a), def(d), speed(s) {}

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

	void learnSkill(Skill* s) { skillList.push_back(s); }
	vector<Skill*>& getSkillList() { return skillList; }

	~Player() 
	{
		for (auto s : skillList) delete s;
	}

	void useMp(int amout)
	{
		currentMp -= amout;

	};

	void PlayerDie()
	{
		cout << endl << getName() << "님이 쓰러졌습니다." << endl;
		cout << "Game Over" << endl;
	}

	string getName() { return name; }
	int getMaxHp() { return maxHp; }
	int getCurrentHp() { return currentHp; }
	int getCurrentMp() { return currentMp; }
	int getAtt() { return att; }
	int getSpeed() { return speed; }
};

class Monster
{
private:
	string name;
	int exp;
	int hp;
	int mp;
	int att;
	int def;
	int speed;

public:
	Monster(string n, int ex, int h, int m, int a, int d, int s) : name(n), exp(ex), hp(h), mp(m), att(a), def(d), speed(s) {}

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
	Slime() : Monster("슬라임", 10, 20, 10, 5, 5, 3) {}
};

class Ork : public Monster
{
public:
	Ork() : Monster("오크", 30, 20, 5, 1000, 5, 2) {}
};

class Wolf : public Monster
{
public:
	Wolf() : Monster("늑대", 50, 15, 20, 30, 10, 15) {}

};

class Skill
{
protected:
	string name;
	int mpConsume;

public:
	Skill(string n, int m) : name(n), mpConsume(m) {}

	virtual ~Skill() {}

	virtual bool useSkill(Player& p, Monster& m) = 0;

	string getName() { return name; }
	int getMpConsume() { return mpConsume; }

};

class Fireball : public Skill
{
public:
	Fireball() : Skill("파이어볼", 20) {}

	bool useSkill(Player& p, Monster& m) override
	{
		if (p.getCurrentMp() >= mpConsume)
		{
			p.useMp(mpConsume);

			int damage = p.getAtt() + 20;
			int finalDmg = m.takeDamage(damage);
			cout << mpConsume << "MP를 사용해 " << getName() << "을 사용합니다" << endl << endl;
			cout << m.getName() << "에게 " << finalDmg << "의 피해!" << endl;
			cout << m.getName() << " 남은 HP: " << m.getHp() << endl;
			return true;

		}
		else
		{
			cout << "MP가 부족합니다." << endl;
			return false;
		}
	}
};

class Strike : public Skill
{
public:
	Strike() : Skill("강타", 50) {}

	bool useSkill(Player& p, Monster& m) override
	{
		if (p.getCurrentMp() >= mpConsume)
		{
			p.useMp(mpConsume);

			int damage = p.getAtt() * 2;
			int finalDmg = m.takeDamage(damage);
			cout << mpConsume << "MP를 사용해 " << getName() << "을(를) 사용합니다" << endl << endl;
			cout << m.getName() << "에게 " << finalDmg << "의 피해!" << endl;
			cout << m.getName() << " 남은 HP: " << m.getHp() << endl;
			return true;

		}
		else
		{
			cout << "MP가 부족합니다." << endl;
			return false;
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

	void wait()
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cin.get();
	}

	bool playerBattle(Player& p, Monster& m, int turn)
	{
		int skillSelect;
		cout << "-" << turn << "번째 " << p.getName() << "의 턴-" << endl << endl;
		cout << "1.공격 2.스킬: ";
		cin >> skillSelect;

		switch (skillSelect)
		{
		case 1:
		{
			int mDmg = m.takeDamage(p.getAtt());
			cout << m.getName() << "에게 " << mDmg << "의 피해를 입혔습니다." << endl;
			cout << m.getName() << " HP: " << m.getHp() << endl;
			break;
		}

		case 2:
		{
			auto& skills = p.getSkillList();
			cout << "\n[ 스킬 목록 ]" << endl;
			for (int i = 0; i < skills.size(); ++i) {
				cout << i + 1 << ". " << skills[i]->getName() << " (MP: " << skills[i]->getMpConsume() << ")" << endl;
			}
			cout << "0. 취소" << endl;
			cout << "번호 선택: ";

			int subSelect;
			cin >> subSelect;

			if (subSelect == 0) return false; // 취소 시 다시 1.공격/2.스킬 메뉴로
			if (subSelect < 1 || subSelect > skills.size()) return false;

			if (!skills[subSelect - 1]->useSkill(p, m)) return false;

			break;
		}

		default:
			cout << "잘못된 입력입니다." << endl;
			return false;
		}

		if (m.getHp() <= 0) {
			m.MonsterDie();
			return true;
		}

		return true;
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
					cout << "전투를 빠져나옵니다." << endl << endl;
					wait();
					return true;
				}
				else
				{
					cout << m.getName() << "에게 따라잡힙니다." << endl << endl;
					battleMessage = false;
					return false;
				}
				return false;
			}
			else if (battleRun == 2)
			{
				cout << "전투를 계속합니다." << endl << endl;
				battleMessage = false;
				return false;
			}
			else
			{
				cout << "잘못된 입력입니다." << endl;
				return false;
			}
		}
		return false;
	}

	void battle(Player& p, Monster& m)
	{
		int turn = 1;
		battleMessage = true;
		cout << m.getName() << "와(과)의 전투를 시작합니다..." << endl << endl;

		while (p.getCurrentHp() > 0 && m.getHp() > 0)
		{
			if (run(p, m)) break;

			if (p.getSpeed() >= m.getSpeed()) // 플레이어 선공
			{
				if (!playerBattle(p, m, turn)) continue; // mp 부족으로 스킬 사용 실패시 다시 선택화면
				if (m.getHp() <= 0) break;
				wait(); 

				if (monsterBattle(p, m, turn)) break;
				wait(); 
			}
			else // 몬스터 선공
			{
				if (monsterBattle(p, m, turn)) break;
				wait();

				// 플레이어의 반격: 실패 시 다시 플레이어 턴부터 시도해야 하므로 continue
				if (!playerBattle(p, m, turn)) continue;
				if (m.getHp() <= 0) break;
				wait();
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

	Player player(nickname, 1, 0, 100, 100, 100, 100, 20, 20, 10); // 레벨 경험치 최대체력 현재체력 최대마나 현재마나 공격력 방어력 스피드 
	player.learnSkill(new Fireball());
	player.learnSkill(new Strike());
	cout << nickname << "님 환영합니다." << endl;

	while (true)
	{
		cout << endl;
		cout << "┌─────────────┐" << endl;
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