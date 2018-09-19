#pragma once

template <class partnerType>
class STATE_MACHIN
{
private:
	PLAYER* lpAllyPlayer;	  //  �Ǐ]����v���C���[
	PLAYER* lpOpponentPlayer; //  ����̃v���C���[

	partnerType* lpPartner; //  ����FMS(�L���X�e�[�g�}�V�[��)�����L����p�[�g�i�[
	PARTNER_STATE<partnerType>* lpCurrentState; //  ���s����X�e�[�g
	PARTNER_STATE<partnerType>* lpPreviousState;//  ���O�̃X�e�[�g�L�^
	PARTNER_STATE<partnerType>* lpGlobalState;  //  �O���[�o���X�e�[�g

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

	//  ���ꂼ��̃X�e�[�g�̃Z�b�^�[
	void SetCurrentState(PARTNER_STATE<partnerType>* s) { lpCurrentState = s; }
	void SetPreviousState(PARTNER_STATE<partnerType>* s) { lpPreviousState = s; }
	void SetGlobalState(PARTNER_STATE<partnerType>* s) { lpGlobalState = s; }

	//  ���ꂼ��̃Q�b�^�[
	PARTNER_STATE<partnerType>* GetCurrentState()const { return lpCurrentState; }
	PARTNER_STATE<partnerType>* GetPreviousState()const { return lpPreviousState; }
	PARTNER_STATE<partnerType>* GetGlobalState()const { return lpGlobalState; }

	//  �X�e�[�g���s
	void Update()
	{
		if (lpGlobalState) lpGlobalState->Execute(lpPartner);
		if (lpCurrentState) lpCurrentState->Execute(lpPartner);
	}

	//  �X�e�[�g�ύX
	void ChangeState(PARTNER_STATE<partnerType>* newState)
	{
		//  ���݂̃X�e�[�g�L�^
		lpPreviousState = lpCurrentState;

		//  ���݂̃X�e�[�g�̏I���֐����s
		lpCurrentState->Exit(lpPartner);

		//  �X�e�[�g�ύX
		lpCurrentState = newState;

		//  �v���C���[�̃|�C���^����Ȃ���
		lpCurrentState->SetAllyPlayer(lpAllyPlayer);
		lpCurrentState->SetOpponentPlayer(lpOpponentPlayer);

		//  ���݂̃X�e�[�g�̊J�n�֐����s
		lpCurrentState->Enter(lpPartner);
	}

	//  ���O�̃X�e�[�g�ɖ߂�܂�
	void ChangePreviousState()
	{
		//  �L�^���Ă������O�̃X�e�[�g�ɕύX
		ChangeState(lpPreviousState);
	}

	//bool ChackState(const PARTNER_STATE<partnerType>& state)
	//{
	//	if (*lpCurrentState == state) return true;
	//	else return false;
	//}
};