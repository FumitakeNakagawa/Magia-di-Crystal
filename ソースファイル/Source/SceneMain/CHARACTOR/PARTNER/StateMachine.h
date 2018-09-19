#pragma once

template <class partnerType>
class STATE_MACHIN
{
private:
	PLAYER* lpAllyPlayer;	  //  追従するプレイヤー
	PLAYER* lpOpponentPlayer; //  相手のプレイヤー

	partnerType* lpPartner; //  このFMS(有限ステートマシーン)を所有するパートナー
	PARTNER_STATE<partnerType>* lpCurrentState; //  実行するステート
	PARTNER_STATE<partnerType>* lpPreviousState;//  直前のステート記録
	PARTNER_STATE<partnerType>* lpGlobalState;  //  グローバルステート

public:
	STATE_MACHIN(partnerType* partner, PARTNER_STATE<partnerType>* s, PARTNER_STATE<partnerType>* s2, PLAYER* p, PLAYER* p2) :lpPartner(partner), lpCurrentState(s),
		lpPreviousState(nullptr), lpGlobalState(s2), lpAllyPlayer(p), lpOpponentPlayer(p2)
	{
		if (lpCurrentState)
		{
			lpCurrentState->SetAllyPlayer(lpAllyPlayer);
			lpCurrentState->SetOpponentPlayer(lpOpponentPlayer);
		}
		if (lpGlobalState)
		{
			lpGlobalState->SetAllyPlayer(lpAllyPlayer);
			lpGlobalState->SetOpponentPlayer(lpOpponentPlayer);
		}
	};

	~STATE_MACHIN()
	{
		lpAllyPlayer = nullptr;
		lpOpponentPlayer = nullptr;
		lpPartner = nullptr;
		lpCurrentState = nullptr;
		lpPreviousState = nullptr;
		lpGlobalState = nullptr;
	}

	//  それぞれのステートのセッター
	void SetCurrentState(PARTNER_STATE<partnerType>* s) { lpCurrentState = s; }
	void SetPreviousState(PARTNER_STATE<partnerType>* s) { lpPreviousState = s; }
	void SetGlobalState(PARTNER_STATE<partnerType>* s) { lpGlobalState = s; }

	//  それぞれのゲッター
	PARTNER_STATE<partnerType>* GetCurrentState()const { return lpCurrentState; }
	PARTNER_STATE<partnerType>* GetPreviousState()const { return lpPreviousState; }
	PARTNER_STATE<partnerType>* GetGlobalState()const { return lpGlobalState; }

	//  ステート実行
	void Update()
	{
		if (lpGlobalState) lpGlobalState->Execute(lpPartner);
		if (lpCurrentState) lpCurrentState->Execute(lpPartner);
	}

	//  ステート変更
	void ChangeState(PARTNER_STATE<partnerType>* newState)
	{
		//  現在のステート記録
		lpPreviousState = lpCurrentState;

		//  現在のステートの終了関数実行
		lpCurrentState->Exit(lpPartner);

		//  ステート変更
		lpCurrentState = newState;

		//  プレイヤーのポインタ入れなおし
		lpCurrentState->SetAllyPlayer(lpAllyPlayer);
		lpCurrentState->SetOpponentPlayer(lpOpponentPlayer);

		//  現在のステートの開始関数実行
		lpCurrentState->Enter(lpPartner);
	}

	//  直前のステートに戻ります
	void ChangePreviousState()
	{
		//  記録していた直前のステートに変更
		ChangeState(lpPreviousState);
	}

	//bool ChackState(const PARTNER_STATE<partnerType>& state)
	//{
	//	if (*lpCurrentState == state) return true;
	//	else return false;
	//}
};