/*!
@file TutorialStage.h
@brief Derived WaveStage
*/

#include "stdafx.h"

namespace basecross {

    class TutorialProgressFrame;
    class TutorialProgress;
    class TutorialDone;
    class TutorialDescription;
    class TutorialDialog;

    class TutorialStage : public WaveStageBase {
    protected:
        enum TutorialPhase {
            Tutorial_Start,
            //移動とカメラ操作
            Tutorial_MoveAndCamera,
            //回避
            Tutorial_Evade,
            //ダッシュ
            Tutorial_KeepRunning,
            //近接攻撃
            Tutorial_HitTheCombos,
            //遠隔攻撃
            Tutorial_ShootFromAfar,
            //殺せ！
            Tutorial_KillAsYouLike,
            //クリア
            Tutorial_Cleared
        };
        TutorialPhase m_tutorialPhase = Tutorial_Start;

        //UI
        weak_ptr<TutorialDialog> m_dialog;
        weak_ptr<TutorialProgressFrame> m_bar[2];

        //チュートリアル進行状況(0～1)
        float m_progress[2] = {0, 0};
        const float m_tutorialMoveRequired = .025f;
        const float m_tutorialCameraRequired = .2f;
        const int m_enemyNum = 5;
        const int m_enemyNumFinalPhase = 8;
        Vec3 m_enemySpawnPos[8] = { Vec3(0.0f, 10.0f, -245.0f),
        Vec3(15.0f, 10.0f, -250.0f),
        Vec3(20.0f, 10.0f, -265.0f),
        Vec3(15.0f, 10.0f, -280.0f),
        Vec3(0.0f, 10.0f, -285.0f),
        Vec3(-15.0f, 10.0f, -285.0f),
        Vec3(-20.0f, 10.0f, -265.0f),
        Vec3(-15.0f, 10.0f, -250.0f) };

        const float m_tutorialEvadeRequired = .55f;
        const float m_tutorialDashRequired = .25f;
        const float m_tutorialAttackRequired = .1f;
        const float m_tutorialAttackRequiredEx = .35f;
        const float m_tutorialShotRequired = .25f;

        //チュートリアルの進行状況を監視
        void UpdateTutorialPhase();
        //次のチュートリアルに移行
        void ChangeTutorialPhase(TutorialPhase phase);

        bool ConsiderGameClear() override;

        void DisplayDialog(wstring key, shared_ptr<TutorialDialog> ui);

        void UpdateGamePhase() override;

    public:
        //構築と破棄
        TutorialStage() : WaveStageBase() {}
        virtual ~TutorialStage() {}
        //初期化
        virtual void OnCreate()override;
        virtual void OnUpdate()override;
    };

    //=============================================================================
    // ベース
    //=============================================================================

    class TutorialSpriteBase : public MyGameObject {
    protected:
        vector<VertexPositionColorTexture> m_vertices;
        shared_ptr<PCTSpriteDraw> m_drawComp;
        wstring m_resKey;

        Vec3 m_addPos;

    public:
        //構築と破棄
        TutorialSpriteBase(shared_ptr<Stage>& stagePtr, wstring resKey)
            : MyGameObject(stagePtr),
            m_resKey(resKey)
        {
        }
        ~TutorialSpriteBase() {}
        //初期化
        //virtual void OnCreate() override;
    };

    //=============================================================================
    // チュートリアルの内容
    //=============================================================================

    class TutorialDescription : public TutorialSpriteBase {

        //元画像の1/5
        float m_width;
        float m_height;

        weak_ptr<TutorialSpriteBase> m_parent;

    public:
        //構築と破棄
        TutorialDescription(shared_ptr<Stage>& stagePtr, wstring resKey, Vec2 size,
            shared_ptr<TutorialSpriteBase> parent)
            : TutorialSpriteBase(stagePtr, resKey),
            m_width(size.x),
            m_height(size.y),
            m_parent(parent)

        {
        }
        ~TutorialDescription() {}
        //初期化
        virtual void OnCreate() override;

        void SetUpVertices() {
            Col4 color(1, 1, 1, 1);

            m_vertices = {
                {Vec3(0, m_height, 0.0f), color, Vec2(0, 0)},
                {Vec3(m_width, m_height, 0.0f), color, Vec2(1, 0)},
                {Vec3(0, 0, 0.0f), color, Vec2(0, 1)},
                {Vec3(m_width, 0, 0.0f), color, Vec2(1, 1)},
            };

            m_addPos = Vec3(-m_width - 10, m_height / 2, 0);
            SetPosition(m_addPos);

        }

        void ChangeResource(wstring resKey, Vec2 size) {
            m_resKey = resKey;
            m_width = size.x;
            m_height = size.y;

            SetUpVertices();
            m_drawComp->UpdateVertices(m_vertices);
            m_drawComp->SetTextureResource(m_resKey);
        }
    };

    //=============================================================================
    // OKの表示
    //=============================================================================

    class TutorialDone : public TutorialSpriteBase {

        //元画像の1/5
        float m_width = 43.4f;
        float m_height = 11.6f;

        weak_ptr<TutorialSpriteBase> m_parent;

    public:
        //構築と破棄
        TutorialDone(shared_ptr<Stage>& stagePtr, wstring resKey, shared_ptr<TutorialSpriteBase> parent)
            : TutorialSpriteBase(stagePtr, resKey),
            m_parent(parent)
        {
        }
        ~TutorialDone() {}
        //初期化
        virtual void OnCreate() override;

        void OnTutorialCompleted(bool active) {
            m_drawComp->SetDrawActive(active);
        }

        bool GetDrawActive() {
            return m_drawComp->GetDrawActive();
        }
    };

    //=============================================================================
    // ゲージ
    //=============================================================================

    class TutorialProgress : public TutorialSpriteBase {

        //下側の頂点をずらす
        float m_width = 140.0f;
        float m_height = 20.0f;
        float m_bottomSlip = 15.5f;

        //0.0～1.0
        float m_percentage = 1.0f;
        float m_percentageDisplay = 0.0f;
        const float m_progressSpeed = 0.5f;

        weak_ptr<TutorialSpriteBase> m_parent;
        weak_ptr<TutorialDone> m_done;

    public:
        //構築と破棄
        TutorialProgress(shared_ptr<Stage>& stagePtr, wstring resKey, shared_ptr<TutorialSpriteBase> parent)
            : TutorialSpriteBase(stagePtr, resKey),
            m_parent(parent)
        {
        }
        ~TutorialProgress() {}
        //初期化
        virtual void OnCreate() override;
        virtual void OnUpdate() override;

        void SetPercentage(float percent) {
            m_percentage = percent;

            if (m_percentage >= 1.0f) {
                m_done.lock()->OnTutorialCompleted(true);
            }
            else {
                m_done.lock()->OnTutorialCompleted(false);
            }
        }

        //子オブジェクトも含めて表示/非表示
        void SetDrawActive(bool active) {
            m_drawComp->SetDrawActive(active);
            m_done.lock()->SetDrawActive(active);
        }
    };

    //=============================================================================
    // ゲージフレーム
    //=============================================================================

    class TutorialProgressFrame : public TutorialSpriteBase {

        //元画像の1/5
        float m_width = 248.2;
        float m_height = 27.4f;

        weak_ptr<TutorialProgress> m_bar;
        weak_ptr<TutorialDescription> m_desc;

        int m_descNum = 0;

    public:
        //構築と破棄
        TutorialProgressFrame(shared_ptr<Stage>& stagePtr, wstring resKey, Vec3 pos)
            : TutorialSpriteBase(stagePtr, resKey)
        {
            m_addPos = pos;
        }
        ~TutorialProgressFrame() {}
        //初期化
        virtual void OnCreate() override;

        void ChangeDescription(int num) {
            switch (num) {
            case 1:
                m_desc.lock()->ChangeResource(L"Tuto_barTxt1", Vec2(87.2f, 21.4f));
                break;
            case 2:
                m_desc.lock()->ChangeResource(L"Tuto_barTxt2", Vec2(131.8f, 21.8f));
                break;
            case 3:
                m_desc.lock()->ChangeResource(L"Tuto_barTxt3", Vec2(198.0f, 21.8f));
                break;
            case 4:
                m_desc.lock()->ChangeResource(L"Tuto_barTxt4", Vec2(194.0f, 22.4f));
                break;
            case 5:
                m_desc.lock()->ChangeResource(L"Tuto_barTxt5", Vec2(152.0f, 22.6f));
                break;
            case 6:
                m_desc.lock()->ChangeResource(L"Tuto_barTxt6", Vec2(220.0f, 21.8f));
                break;
            case 7:
                m_desc.lock()->ChangeResource(L"Tuto_barTxt7", Vec2(151.6f, 21.6f));
                break;
            default:
                break;
            }
        }

        void SetPercent(float perc) {
            m_bar.lock()->SetPercentage(perc);
        }

        //子オブジェクトも含めて表示/非表示
        void SetDrawActive(bool active) {
            m_drawComp->SetDrawActive(active);
            m_bar.lock()->SetDrawActive(active);
            m_desc.lock()->SetDrawActive(active);
        }
    };

    //=============================================================================
    // ダイアログ
    //=============================================================================

    class TutorialDialog : public TutorialSpriteBase {
    protected:
        KEYBOARD_STATE m_key;
        CONTROLER_STATE m_pad;

        vector<VertexPositionColorTexture> m_movedVertices;

        //---- 移動処理 -----------------------------------
        //補間法
        Lerp::rate m_lerpRate;
        //経過した時間
        float m_elapsedTime = 0;
        //完了するまでの時間
        float m_arriveTime = 0;
        //開始位置
        Vec2 m_originPos;
        //現在位置
        Vec2 m_currentPos;
        //目標位置
        Vec2 m_targetPos;
        //移動完了
        bool m_isMoveDone = true;

        //---- カラー操作 ---------------------------------
        Lerp::rate m_color_lerpRate;
        //経過した時間
        float m_color_elapsedTime = 0;
        //完了するまでの時間
        float m_color_arriveTime = 0;
        //開始カラー
        Col4 m_originColor;
        //目標カラー
        Col4 m_targetColor;
        //カラー変え終わり
        bool m_changeColorDone = true;

        bool m_isInvisible = false;

        void MoveTowardTargetPos();
        void UpdateColor();

    public:
        //構築と破棄
        TutorialDialog(shared_ptr<Stage>& stagePtr, wstring resKey, Vec2 pos, Col4 color) :
            TutorialSpriteBase(stagePtr, resKey),
            m_originPos(pos),
            m_currentPos(pos),
            m_targetPos(pos),
            m_originColor(color),
            m_targetColor(color),
            m_lerpRate(Lerp::rate::Linear),
            m_color_lerpRate(Lerp::rate::Linear)
        {
        }
        ~TutorialDialog() {}
        //初期化
        virtual void OnCreate() override;
        virtual void OnUpdate() override;

        void SetResource(wstring key) {
            m_resKey = key;
            m_drawComp->SetTextureResource(m_resKey);
        }

        //移動が完了したらtrueが返る
        bool GetMoveDone() {
            return m_isMoveDone;
        }
        bool GetChangeColorDone() {
            return m_changeColorDone;
        }

        bool IsInvisible() {
            return m_isInvisible;
        }

        //移動処理開始
        void SetMovePos(Vec2 pos, float time, Lerp::rate lerp) {
            m_originPos = m_currentPos;
            m_targetPos = pos;
            m_arriveTime = time;
            m_lerpRate = lerp;
            m_elapsedTime = 0;
            m_isMoveDone = false;
        }
        void SetMovePos(Vec2 pos, float time) {
            SetMovePos(pos, time, m_lerpRate);
        }

        //色変更処理開始
        void SetColorChange(Col4 color, float time, Lerp::rate lerp) {
            //代表して最初の点から取る
            m_originColor = m_drawComp->GetDiffuse();
            m_targetColor = color;
            m_color_arriveTime = time;
            m_color_lerpRate = lerp;
            m_color_elapsedTime = 0;
            m_changeColorDone = false;
        }
        void SetColorChange(Col4 color, float time) {
            SetColorChange(color, time, m_color_lerpRate);
        }
    };

}