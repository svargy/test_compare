class BS_AH64D_Apache_Base: House
{
    Particle m_Ptc_RotorDmg, m_Ptc_BladeDmg, m_Ptc_BladeFire;
    Particle m_Ptc_MuzzleFlash, m_Ptc_MuzzleSmoke, m_Ptc_TailFire;
    Particle m_Ptc_Exhaust1, m_Ptc_Exhaust2;

    #ifdef SERVER
    ref BS_FlightController m_PatrolController;
    #endif

    #ifdef EXPANSIONMODNAVIGATION
    string m_ExpansionMarkerUID;
    #endif

    #ifdef LBmaster_Groups
    int m_LBMarkerUID = 0;
    #endif

    float m_MarkerUpdateTimer = 0.0;
    const float MARKER_UPDATE_INTERVAL = 4.0;
    protected vector m_LastMarkerPos;
    protected bool m_MarkerPlaced = false;
    static const float MARKER_MIN_MOVE = 40.0;

    ref array<SurvivorBase> m_ActiveHostiles;
    SurvivorBase m_PrimaryTarget;
    float m_TimeSinceTargetAcquired;
    bool m_HasLineOfSight = false;

    protected int m_CrashPhase = 0;
    protected float m_CrashPhaseTimer = 0.0;
    protected float m_CrashTotalTime = 0.0;
    protected vector m_CrashVelocity = "0 0 0";
    protected vector m_CrashAngularVel = "0 0 0";
    protected float m_InitialForwardSpeed = 0.0;
    protected vector m_InitialDirection = "0 0 1";  
    protected float m_CrashYaw = 0.0;
    protected float m_CrashPitch = 0.0;
    protected float m_CrashRoll = 0.0;
    protected float m_SpinDirection = 1.0;
    protected float m_TailRotorFailure = 0.0;  

    protected float m_LiftCoefficient = 1.0;
    protected float m_DragCoefficient = 0.3;
    protected bool m_MainRotorFailed = false;
    protected bool m_TailRotorFailed = false;
    protected float m_EngineFireIntensity = 0.0;

    protected bool m_HasBounced = false;
    protected int m_BounceCount = 0;
    protected vector m_LastGroundNormal = "0 1 0";
    protected Particle m_CrashSmokeHeavy;
    protected Particle m_CrashFireEngine;
    protected Particle m_CrashFireTail;
    protected Particle m_CrashDebris;
    protected EffectSound m_CrashAlarmSound;
    protected EffectSound m_CrashWindSound;
    protected float m_LastTrailSpawnTime = 0.0;
    static const vector POS_MAIN_ROTOR = "0.0 2.5 -0.5";  
    static const vector POS_TAIL_ROTOR = "0.0 1.8 -8.5";  
    static const vector POS_GUN_MUZZLE = "0.0 -1.5 4.5";  
    const vector POS_TAIL_LIGHT = "0.0 2.0 -8.8";  
    static const vector POS_WRECK_FIRE = "0.0 1.0 -1.0";

    protected bool m_BS_PointsReady = false;
    protected bool m_BS_HasGunPivot = false;
    protected bool m_BS_HasMissileRails = false;
    protected vector m_BS_PtMuzzle;
    protected vector m_BS_PtGunPivot;
    protected vector m_BS_PtMainRotor;
    protected vector m_BS_PtTailRotor;
    protected vector m_BS_PtTailLight;
    protected vector m_BS_PtFlareL;
    protected vector m_BS_PtFlareR;
    protected vector m_BS_PtMissileL;
    protected vector m_BS_PtMissileR;
    static const vector POS_MISSILE_LEFT = "-2.5 -1.8 17.0";
    static const vector POS_MISSILE_RIGHT = "2.5 -1.8 17.0";

    static const float CRASH_GRAVITY = 9.81;
    static const float CRASH_AIR_DENSITY = 1.225;
    static const float CRASH_HELI_MASS = 11000.0;
    static const float CRASH_ROTOR_AREA = 250.0;
    static const float CRASH_TERMINAL_VELOCITY = 80.0;
    static const float CRASH_BOUNCE_RESTITUTION = 0.15;
    static const float CRASH_FRICTION = 0.8;

    static const float PHASE_INITIAL_DURATION = 0.5;
    static const float PHASE_SPIN_DURATION = 1.5;
    static const float PHASE_FREEFALL_MIN_HEIGHT = 50.0;

    protected int m_CrashMode = 0;
    protected float m_CrashSpinTarget = 250.0;
    protected float m_CrashSpinTau = 0.9;
    protected float m_CrashDescent = 18.0;
    protected float m_CrashDescentTau = 1.4;
    protected float m_CrashHorizTau = 6.0;
    protected float m_CrashPitchTarget = 9.0;
    protected float m_CrashRollTarget = 16.0;
    protected float m_CrashYawRate = 0.0;
    protected float m_CrashPitchRate = 0.0;
    protected float m_CrashRollRate = 0.0;
    protected float m_CrashWobblePhase = 0.0;
    protected float m_CrashBottom = 2.0;
    protected bool m_CrashSliding = false;
    protected float m_CrashSlideTime = 0.0;
    protected float m_CrashLastTreeTime = 0.0;
    protected float m_CrashLastDustTime = 0.0;
    protected bool m_CrashImpactPending = false;
    protected float m_CrashImpactTimer = 0.0;
    protected vector m_CrashImpactPos;
    protected float m_CrashImpactGround = 0.0;
    static const float CRASH_SLIDE_MAX_IMPACT = 30.0;
    static const float CRASH_SLIDE_DECEL = 12.0;
    static const float CRASH_SLIDE_MIN_SPEED = 10.0;
    static const float CRASH_SLIDE_MAX_TIME = 2.0;
    static const float CRASH_ATT_W = 2.6;
    static const float CRASH_ATT_Z = 0.8;
    static const float CRASH_ACC_MAX = 10.8;
    static const float CRASH_IMPACT_HOLD = 0.08;
    static const float CRASH_ATT_MAX = 50.0;
    static const float CRASH_SLIDE_MAX_TOTAL = 50.0;
    protected int m_CrashLastTickMS = -1;
    protected float m_CrashSlideDecel = 7.0;

    static const float TAIL_BREAK_DELAY = 0.5;
    static const float TAIL_FLING_MAX = 18.0;
    static const float TAIL_RPC_DISTANCE = 1500.0;
    static const vector TAIL_ORIGIN_DEFAULT = "0 1 -7.3";
    static const vector TAIL_CUT_DEFAULT = "0 0.47 -4.239";
    protected bool m_BS_TailBreakPending = false;
    protected bool m_BS_TailBroken = false;
    protected Particle m_BS_StumpSmoke;
    protected bool m_CrashOverWater = false;

    static const string ANIM_GUN_YAW = "turret_yaw";
    static const string ANIM_GUN_PITCH = "turret_pitch";
    static const float TARGET_AIM_OFFSET = 1.5;
    protected float m_LastClientLoopTime = 0.0;
    protected bool m_IsSuppressing = false;
    protected vector m_SuppressionTargetPos;
    protected float m_SuppressionTimer = 0.0;
    protected float m_DamageAccumulator = 0.0;
    protected float m_LastHitTime = 0.0;
    protected float m_NoLOSWhileDamagedTimer = 0.0; 
    protected bool m_TakingDamageRecently = false;   

    protected float m_TargetFocusTime = 0.0;
    // --- VARIABLES DE ESTADO ---
    float m_FlightSpeed;
    float m_Altitude;
    float m_PrevYaw;
    float m_HeliTimer;
    float m_PatrolElapsed;
    int m_FlightState;
    bool m_IsCrashing = false;
    bool m_IsCrashed = false;
    bool m_GunReady;
    vector m_SpawnOrigin;

    // --- DISPARO ---
    float m_LastHitTimeMS;
    float m_LastFireTimeMS;
    float m_WeaponHeat = 0.0;
    bool m_WeaponOverheated = false;
    float m_GunYawDeg = 0.0;
    float m_GunPitchDeg = 0.0;
    float m_MissileTimer = 0.0;
    bool m_FireLeftPod = true;
    float m_FlareTimer = 0.0;

    // --- ANIMACIÓN ---
    protected float m_RotorAnimationPosition = 0.0;
    protected float m_RotorSpeed = 8.0;
    protected float m_RotorAnimPhase = 0.0;
    
    // --- INTERPOLACIÓN ---
    protected vector m_InterpStartPos;
    protected vector m_InterpTargetPos;
    protected vector m_InterpStartOri;
    protected vector m_InterpTargetOri;
    protected float m_InterpTime = 0.0;
    protected float m_InterpDuration = 0.1;
    protected bool m_HasPreviousPos = false;
    
    // --- CRASH ---
    float m_CrashStartTime = 0;
    float m_FallSpeed = 0.0;
    float m_FallRotation = 0.0;
    float m_CurrentSpinSpeed = 0.0;

    // --- CLIENTE: Banking visual ---
    protected float m_LastClientYaw = 0.0;
    protected float m_SmoothedTurnRate = 0.0;
    protected float m_VisualBank = 0.0;
    protected float m_LastUpdateTime = 0.0;
    
    // --- CLIENTE: Interpolación suave ---
    protected vector m_ServerPos;
    protected vector m_ServerOri;
    protected vector m_ServerVel;
    protected vector m_VisualPos;
    protected vector m_VisualOri;
    protected vector m_LastSetPos;

    protected bool   m_SmoothMovement = false;
    protected vector m_SyncVel;
    protected bool m_SyncVelValid = false;
    protected vector m_NetPos;
    protected vector m_PrevNetPos;
    protected float  m_NetTimeSmooth = 0.0;
    protected float  m_PrevNetTime = 0.0;
    protected float  m_NetArrival = 0.0;
    protected vector m_NetOri;
    protected vector m_PrevNetOri;
    protected float  m_OriTime = 0.0;
    protected float  m_PrevOriTime = 0.0;
    protected vector m_NetVel;
    protected float  m_NetYawRate = 0.0;
    protected bool   m_NetInit = false;
    protected vector m_LastSetOri;

    protected float  m_LastVelSyncTime = -1.0;
    protected float  m_LastSentSpeed = 0.0;
    protected float  m_LastSentVertical = 0.0;
    static const float VEL_SYNC_INTERVAL = 0.2;
    static const float VEL_SYNC_MIN_INTERVAL = 0.035;
    static const float VEL_SYNC_SPEED_STEP = 0.5;
    static const float VEL_SYNC_VERTICAL_STEP = 1.0;
    static const float VEL_SYNC_DISTANCE = 1500.0;
    protected vector m_TargetPos;
    protected vector m_TargetOri;
    protected float m_LastServerTime = 0.0;

    // --- LUCES ---
    BS_HeliBlinkerLight m_BS_HeliBlinkerLight;
    BS_HeliSearchLight m_BS_HeliSearchLight;
    float m_BS_LightBrightness = 35.0;
    float m_BS_LightRadius = 500.0;
    float m_BS_LightAngle = 45.0;
    bool m_BS_LightEnabled = true;

    protected int m_BS_BurstCount = 0;
    protected float m_BS_BurstPauseUntilMS = 0.0;

    bool m_BS_Leaving = false;
    vector m_BS_LeavePoint;
    bool m_BS_LeftArea = false;
    string m_BS_LastAttackerName = "";
    float m_BS_LastAttackerMS = 0.0;

    protected float m_CustomMaxHealth = 0; 
    protected float m_CustomCurrentHealth = 0; 
    string m_PresetName;

    ref BS_HeliConfig m_Config;

    // --- SONIDO ---
    protected EffectSound m_FlyingSound;

    protected int m_AITickCounter = 0;
    protected int m_MarkerTickCounter = 0;
    protected float m_LastAltitudeBoostCheck = 0;
    protected float m_CachedAltitudeBoost = 0;
    protected vector m_LastAltitudeCheckPos = "0 0 0";
    
    // --- NUEVO: Patrol Aggro ---
    protected float m_PatrolScanTimer = 0.0;
    protected SurvivorBase m_PatrolAggroTarget;  
    
    static const int AI_UPDATE_INTERVAL = 4;
    static const int MARKER_UPDATE_TICKS = 160;
    static const int DAMAGE_SYNC_TICKS = 400; // ~10s: reenviar nivel de daño periodicamente
    static const float ALTITUDE_CHECK_INTERVAL = 0.5;
    static const float ALTITUDE_CHECK_MOVE_THRESHOLD = 20.0;
    static const float RPC_MAX_DISTANCE = 600.0;
    static const float DAMAGE_SYNC_DISTANCE = 1200.0; // mayor distancia para sincronizar daño visual

    protected int m_DamageSyncTickCounter = 0;
    protected int m_CurrentVisualDamageLevel = 0; // nivel actual guardado en servidor

    protected ref BS_DifficultyConfig m_Diff;

    BS_HeliConfig GetConfig()
    {
        if (m_Config) return m_Config;
        return BS_HeliConfig.Get();
    }

    BS_DifficultyConfig GetDiff()
    {
        if (!m_Diff)
            m_Diff = GetConfig().GetDifficulty();
        return m_Diff;
    }

    void BS_AH64D_Apache_Base()
    {
        m_ActiveHostiles = new array<SurvivorBase>();
        m_LastUpdateTime = GetGame().GetTickTime();
        
        m_LastHitTimeMS = -1;
        m_LastFireTimeMS = 0;
        m_GunReady = true;

        RegisterNetSyncVariableFloat("m_BS_LightBrightness", 0, 1000, 1);
        RegisterNetSyncVariableFloat("m_BS_LightRadius", 0, 5000, 1);
        RegisterNetSyncVariableFloat("m_BS_LightAngle", 0, 180, 1);
        RegisterNetSyncVariableBool("m_BS_LightEnabled");
    }

    void BS_ApplyServerSettings()
    {
        if (!GetGame().IsServer()) return;
        m_Diff = null;
        BS_HeliConfig lightCfg = GetConfig();
        if (lightCfg && lightCfg.HelicopterSettings && m_CustomMaxHealth <= 0)
        {
            BS_HelicopterSettings armorHs = lightCfg.HelicopterSettings;
            bool scaled = (armorHs.DamageMultiplier != 1.0 || armorHs.BulletDamageMultiplier != 1.0 || armorHs.ExplosiveDamageMultiplier != 1.0);
            if (scaled)
            {
                float engineMaxHp = GetMaxHealth("", "Health");
                SetCustomMaxHealth(engineMaxHp);
            }
        }
        if (lightCfg && lightCfg.SearchLightSettings)
        {
            BS_SearchLightSettings sls = lightCfg.SearchLightSettings;
            m_BS_LightEnabled = sls.Enabled;
            m_BS_LightBrightness = Math.Clamp(sls.Brightness, 0, 1000);
            m_BS_LightRadius = Math.Clamp(sls.Radius, 0, 5000);
            m_BS_LightAngle = Math.Clamp(sls.ConeAngle, 0, 180);
        }
        SetSynchDirty();
        if (!GetGame().IsMultiplayer()) BS_RefreshSearchLight();
    }

    override void OnVariablesSynchronized()
    {
        super.OnVariablesSynchronized();
        BS_RefreshSearchLight();
    }

    void BS_RefreshSearchLight()
    {
        if (!m_BS_LightEnabled)
        {
            if (m_BS_HeliSearchLight) { m_BS_HeliSearchLight.FadeOut(); m_BS_HeliSearchLight = null; }
            return;
        }
        if (m_BS_HeliSearchLight)
            m_BS_HeliSearchLight.BS_Configure(m_BS_LightBrightness, m_BS_LightRadius, m_BS_LightAngle);
        else if (!m_IsCrashing && !m_IsCrashed)
            SetupVisualEffects();
    }

    bool BS_BurstReady(float nowMS)
    {
        return (nowMS >= m_BS_BurstPauseUntilMS);
    }

    void BS_BurstShot(float nowMS)
    {
        BS_DifficultyConfig burstDiff = GetDiff();
        if (!burstDiff || burstDiff.TurretBurstShots <= 0) return;
        m_BS_BurstCount++;
        if (m_BS_BurstCount < burstDiff.TurretBurstShots) return;
        m_BS_BurstCount = 0;
        m_BS_BurstPauseUntilMS = nowMS + burstDiff.TurretBurstCooldown * 1000.0;
    }

    void BS_FinishLeave()
    {
        #ifdef SERVER
        if (m_IsCrashing || m_IsCrashed || m_BS_LeftArea) return;
        BS_Patrol_Log.GetInstance().Info("[" + m_PresetName + "] Helicopter left the area.");
        m_BS_LeftArea = true;
        GetGame().ObjectDelete(this);
        #endif
    }

    #ifdef SERVER
    void BS_AnnounceDestroyedBy()
    {
        BS_HeliConfig killCfg = GetConfig();
        if (!killCfg || !killCfg.Notifications || !killCfg.Notifications.EnableDestroyedByNotification) return;
        float sinceHitMS = GetGame().GetTime() - m_BS_LastAttackerMS;
        if (m_BS_LastAttackerName == "" || sinceHitMS > 30000) return;
        BS_NotificationSettings killNs = killCfg.Notifications;
        BS_NotificationHelper.BroadcastArg(killNs.NotificationTitleKey, killNs.DestroyedByNotificationKey, killNs.NotificationIcon, m_BS_LastAttackerName);
        BS_Patrol_Log.GetInstance().Info("[" + m_PresetName + "] Helicopter destroyed by " + m_BS_LastAttackerName);
    }
    #endif

    void SetCustomMaxHealth(float health)
    {
        m_CustomMaxHealth = health;
        m_CustomCurrentHealth = health;

        float engineMax = GetMaxHealth("", "Health");
        SetHealth("", "Health", engineMax);
    }

    float GetCustomMaxHealth()
    {
        return m_CustomMaxHealth;
    }

    float GetCustomCurrentHealth()
    {
        return m_CustomCurrentHealth;
    }

    void ~BS_AH64D_Apache_Base() 
    {
        if (m_FlyingSound) StopSoundSet(m_FlyingSound);
    }

    override void EEInit()
    {
        super.EEInit();
        
        if (GetGame().IsClient() || !GetGame().IsMultiplayer())
        {
            SetupVisualEffects();
            if (m_FlyingSound) StopSoundSet(m_FlyingSound); 
            PlaySoundSetLoop(m_FlyingSound, "BSPatrol_FlyingSoundSet", 0.5, 0.5);
            
            m_VisualPos = GetPosition();
            m_VisualOri = GetOrientation();
            m_LastSetPos = GetPosition(); 
            m_TargetPos = GetPosition();
            m_TargetOri = GetOrientation();
            
            m_LastVisualDamageLevel = -1;
            
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(RefreshDamageParticles, 5000, true);
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(RefreshFlyingSound, 2000, true);
            
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ClientUpdateLoop, 1, true);
        }

        // --- SERVIDOR ---
        #ifdef SERVER
        if (GetGame().IsServer())
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnUpdate, 25, true);
        }
        #endif
    }
    
    #ifdef SERVER
    void EngagePatrolTarget(SurvivorBase target)
    {
        if (!m_PatrolController || !target) return;
        
        m_PatrolController.m_CombatTarget = target;
        m_PatrolController.m_EngagePosition = target.GetPosition();
        
        m_PatrolController.m_InCombat = true;
        m_PatrolController.m_OnPatrol = false;
        m_PatrolController.m_Disengaging = false;
        m_PatrolController.m_AtWaypoint = false;
        
        m_PatrolController.m_StateTimer = 0;
        m_PatrolController.m_RouteProgress = 0;
    }
    #endif

    SurvivorBase ResolveAttacker(EntityAI source)
    {
        if (!source) return null;
        
        SurvivorBase attacker = SurvivorBase.Cast(source);
        if (attacker) return attacker;
        
        EntityAI owner = EntityAI.Cast(source.GetHierarchyParent());
        if (owner)
            return SurvivorBase.Cast(owner);
        
        return null;
    }

    void ConfigureFlight(vector origin, float cruiseSpeed, float cruiseAltitude)
    {
        m_SpawnOrigin = origin;
        m_FlightSpeed = cruiseSpeed;
        m_Altitude = cruiseAltitude;
        
        vector ori = GetOrientation();
        m_PrevYaw = ori[0];
        m_FlightState = 0;
        m_HeliTimer = 0;
        m_PatrolElapsed = 0;
    }

    void ClientUpdateLoop()
    {
        if (!this) return;
        
        if (m_IsCrashed) 
        {
            StopFlyingSoundClient();
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ClientUpdateLoop);
            return;
        }
        
        float currentTime = GetGame().GetTickTime();
        if (m_LastClientLoopTime == 0.0) m_LastClientLoopTime = currentTime;
        float dt = currentTime - m_LastClientLoopTime;
        m_LastClientLoopTime = currentTime; // siempre actualizar, incluso fuera de rango
        
        if (dt > 0.1) dt = 0.03; 
        if (dt <= 0) dt = 0.001;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
        {
            float distSq = vector.DistanceSq(GetPosition(), player.GetPosition());
            
            if (distSq > 2250000) // > 1500m
            {
                // Si el sonido esta activo y el jugador esta muy lejos, pararlo
                if (m_FlyingSound && distSq > 4000000) // > 2000m: definitivamente fuera de rango de audio
                    StopFlyingSoundClient();
                return;
            }
            
            if (!m_FlyingSound && !m_IsCrashing)
            {
                PlaySoundSetLoop(m_FlyingSound, "BSPatrol_FlyingSoundSet", 0.5, 0.5);
            }
        }

        if (!BS_HasClientRotors())
            AnimateRotors(dt);
        UpdateClientInterpolation(dt);
        UpdateBanking(dt);
    }
    
    void StopFlyingSoundClient()
    {
        if (m_FlyingSound)
        {
            StopSoundSet(m_FlyingSound);
            m_FlyingSound = null;
        }
    }

    void UpdateClientInterpolation(float dt)
    {
        if (m_SmoothMovement)
            UpdateClientInterpolationSmooth(dt);
        else
            UpdateClientInterpolationLegacy(dt);
    }

    static const float FLIGHT_RENDER_DELAY = 0.03;
    static const float CRASH_RENDER_DELAY = 0.03;
    static const float FLIGHT_YAW_RATE_MAX = 120.0;
    static const float CRASH_YAW_RATE_MAX = 420.0;

    void UpdateClientInterpolationSmooth(float dt)
    {
        if (m_IsCrashed) return;

        float now = GetGame().GetTickTime();
        vector enginePos = GetPosition();
        vector engineOri = GetOrientation();

        if (!m_NetInit)
        {
            m_NetPos = enginePos;
            m_PrevNetPos = enginePos;
            m_NetOri = engineOri;
            m_PrevNetOri = engineOri;
            m_NetVel = vector.Zero;
            m_NetYawRate = 0.0;
            m_NetTimeSmooth = now;
            m_PrevNetTime = now - 0.04;
            m_NetArrival = now;
            m_OriTime = now;
            m_PrevOriTime = now - 0.04;
            m_VisualPos = enginePos;
            m_VisualOri = engineOri;
            m_LastSetPos = enginePos;
            m_LastSetOri = engineOri;
            m_NetInit = true;
            return;
        }

        bool sameAsOurs = (vector.DistanceSq(enginePos, m_LastSetPos) <= 0.0004);
        bool sameAsNet = (vector.DistanceSq(enginePos, m_NetPos) <= 0.0004);

        bool haveSyncVel = (m_SyncVel.Length() > 0.05);
        if (m_IsCrashing && m_SyncVelValid)
            haveSyncVel = true;
        vector vel = m_NetVel;
        if (haveSyncVel)
            vel = m_SyncVel;

        bool posPacket = (!sameAsOurs && !sameAsNet);
        if (posPacket)
        {
            float arrivalGap = now - m_NetArrival;
            float jump = vector.Distance(enginePos, m_NetPos);

            float speedNow = vel.Length();
            float motionGap = arrivalGap;
            if (speedNow > 2.0)
                motionGap = jump / speedNow;
            if (motionGap < 0.02)
                motionGap = 0.02;
            if (motionGap > 0.3)
                motionGap = 0.3;

            float expected = m_NetTimeSmooth + motionGap;
            float drift = now - expected;
            float newTime = expected + drift * 0.05;
            if (drift > 0.2 || drift < -0.2)
                newTime = now;

            if (arrivalGap > 0.015)
            {
                vector newVel = (enginePos - m_NetPos) * (1.0 / arrivalGap);
                float newSpeed = newVel.Length();
                if (newSpeed > 90.0)
                    newVel = newVel * (90.0 / newSpeed);
                if (arrivalGap < 1.0)
                    m_NetVel = m_NetVel * 0.5 + newVel * 0.5;
                else
                    m_NetVel = newVel;
            }

            m_PrevNetPos = m_NetPos;
            m_PrevNetTime = m_NetTimeSmooth;
            m_NetPos = enginePos;
            m_NetTimeSmooth = newTime;
            m_NetArrival = now;
        }

        bool oriSameAsOurs = (BS_AngleDistance(engineOri, m_LastSetOri) <= 0.05);
        bool oriSameAsNet = (BS_AngleDistance(engineOri, m_NetOri) <= 0.05);
        if (!oriSameAsOurs && !oriSameAsNet)
        {
            float newOriTime = now;
            if (posPacket)
                newOriTime = m_NetTimeSmooth;
            float oriGap = newOriTime - m_OriTime;
            if (oriGap > 0.015 && oriGap < 1.0)
            {
                float yawDelta = BS_WrapAngle(engineOri[0] - m_NetOri[0]);
                float newRate = yawDelta / oriGap;
                float yawRateMax = FLIGHT_YAW_RATE_MAX;
                if (m_IsCrashing)
                    yawRateMax = CRASH_YAW_RATE_MAX;
                if (newRate > yawRateMax)
                    newRate = yawRateMax;
                if (newRate < -yawRateMax)
                    newRate = -yawRateMax;
                m_NetYawRate = m_NetYawRate * 0.5 + newRate * 0.5;
            }
            m_PrevNetOri = m_NetOri;
            m_PrevOriTime = m_OriTime;
            m_NetOri = engineOri;
            m_OriTime = newOriTime;
        }

        float renderDelay = FLIGHT_RENDER_DELAY;
        if (m_IsCrashing)
            renderDelay = CRASH_RENDER_DELAY;
        float renderTime = now - renderDelay;

        vector predicted;
        float span = m_NetTimeSmooth - m_PrevNetTime;
        if (renderTime <= m_NetTimeSmooth && span > 0.001)
        {
            float lerpT = (renderTime - m_PrevNetTime) / span;
            if (lerpT < 0.0)
                lerpT = 0.0;
            if (lerpT > 1.0)
                lerpT = 1.0;
            predicted = vector.Lerp(m_PrevNetPos, m_NetPos, lerpT);
        }
        else
        {
            float ahead = renderTime - m_NetTimeSmooth;
            if (ahead > 1.0)
                ahead = 1.0;
            if (ahead < 0.0)
                ahead = 0.0;
            predicted = m_NetPos + vel * ahead;
        }

        float err = vector.Distance(m_VisualPos, predicted);
        if (err > 40.0)
        {
            m_VisualPos = predicted;
        }
        else
        {
            float kPos = 1.0 - Math.Pow(Math.EULER, -dt * 30.0);
            m_VisualPos = vector.Lerp(m_VisualPos, predicted, kPos);
        }

        float targetYaw;
        float targetPitch;
        float targetRoll;
        float oriSpan = m_OriTime - m_PrevOriTime;
        if (renderTime <= m_OriTime && oriSpan > 0.001)
        {
            float oriT = (renderTime - m_PrevOriTime) / oriSpan;
            if (oriT < 0.0)
                oriT = 0.0;
            if (oriT > 1.0)
                oriT = 1.0;
            targetYaw = m_PrevNetOri[0] + BS_WrapAngle(m_NetOri[0] - m_PrevNetOri[0]) * oriT;
            targetPitch = m_PrevNetOri[1] + (m_NetOri[1] - m_PrevNetOri[1]) * oriT;
            targetRoll = m_PrevNetOri[2] + (m_NetOri[2] - m_PrevNetOri[2]) * oriT;
        }
        else
        {
            float oriAhead = renderTime - m_OriTime;
            if (oriAhead > 1.0)
                oriAhead = 1.0;
            if (oriAhead < 0.0)
                oriAhead = 0.0;
            targetYaw = m_NetOri[0] + m_NetYawRate * oriAhead;
            targetPitch = m_NetOri[1];
            targetRoll = m_NetOri[2];
        }

        float kOri = 1.0 - Math.Pow(Math.EULER, -dt * 30.0);
        float visYaw = m_VisualOri[0] + BS_WrapAngle(targetYaw - m_VisualOri[0]) * kOri;
        float visPitch = m_VisualOri[1] + (targetPitch - m_VisualOri[1]) * kOri;
        float visRoll = m_VisualOri[2] + (targetRoll - m_VisualOri[2]) * kOri;
        m_VisualOri = Vector(BS_WrapAngle(visYaw), visPitch, visRoll);

        SetPosition(m_VisualPos);
        SetOrientation(m_VisualOri);
        m_LastSetPos = m_VisualPos;
        m_LastSetOri = m_VisualOri;
    }

    static float BS_WrapAngle(float a)
    {
        while (a > 180.0)
            a = a - 360.0;
        while (a <= -180.0)
            a = a + 360.0;
        return a;
    }

    static float BS_AngleDistance(vector a, vector b)
    {
        float total = 0.0;
        for (int i = 0; i < 3; i++)
        {
            float d = a[i] - b[i];
            while (d > 180.0)
                d = d - 360.0;
            while (d < -180.0)
                d = d + 360.0;
            if (d < 0.0)
                d = -d;
            total = total + d;
        }
        return total;
    }

    void UpdateClientInterpolationLegacy(float dt)
    {
        if (m_IsCrashed) return;

        vector enginePos = GetPosition();
        vector engineOri = GetOrientation();

        if (!m_HasPreviousPos)
        {
            m_VisualPos = enginePos;
            m_InterpTargetPos = enginePos;
            m_InterpStartPos = enginePos;
            m_InterpStartOri = engineOri;
            m_InterpTargetOri = engineOri;
            m_HasPreviousPos = true;
            return; 
        }
        
        float distError = vector.Distance(enginePos, m_VisualPos);
        
        if (distError > 0.05)
        {
            m_InterpStartPos = m_VisualPos; 
            m_InterpStartOri = GetOrientation(); 
            m_InterpTargetPos = enginePos;
            m_InterpTargetOri = engineOri;
            m_InterpTime = 0.0;
            
            if (distError > 50.0) m_InterpDuration = 0.05; 
            else if (distError > 35.0) m_InterpDuration = 0.2;
            else m_InterpDuration = 0.3; 
            if (m_IsCrashing && m_InterpDuration > 0.1) m_InterpDuration = 0.1;
        }
        
        m_InterpTime += dt;
        float t = m_InterpTime / m_InterpDuration;
        if (t > 1.0) t = 1.0;
        
        vector newPos = vector.Lerp(m_InterpStartPos, m_InterpTargetPos, t);
        
        float yawStart = m_InterpStartOri[0];
        float yawEnd = m_InterpTargetOri[0];
        float yawDiff = yawEnd - yawStart;
        while (yawDiff > 180) yawDiff -= 360;
        while (yawDiff < -180) yawDiff += 360;
        
        float currentYaw = yawStart + (yawDiff * t);
        float currentPitch = m_InterpStartOri[1] + (m_InterpTargetOri[1] - m_InterpStartOri[1]) * t;
        float currentRoll = m_InterpStartOri[2] + (m_InterpTargetOri[2] - m_InterpStartOri[2]) * t;
        
        m_VisualPos = newPos;
        SetPosition(m_VisualPos);
        SetOrientation(Vector(currentYaw, currentPitch, currentRoll));
    }

    void OnUpdate()
    {
        #ifdef SERVER
        if (GetGame().IsServer())
        {
            if (!BS_HasClientRotors())
            {
                m_RotorAnimPhase += 0.2;
                if (m_RotorAnimPhase > 1) m_RotorAnimPhase -= 1;
                SetAnimationPhase("Engine1", m_RotorAnimPhase);
                SetAnimationPhase("Engine2", m_RotorAnimPhase);
            }

            if (m_IsCrashing) 
            {
                int crashNowMS = GetGame().GetTime();
                float crashElapsed = 0.025;
                if (m_CrashLastTickMS >= 0)
                {
                    float crashDeltaMS = crashNowMS - m_CrashLastTickMS;
                    crashElapsed = crashDeltaMS * 0.001;
                }
                m_CrashLastTickMS = crashNowMS;
                if (crashElapsed > 0.1) crashElapsed = 0.1;
                while (crashElapsed > 0.001 && m_IsCrashing)
                {
                    float crashStep = Math.Min(crashElapsed, 0.025);
                    ApplyCrashPhysics(crashStep);
                    crashElapsed = crashElapsed - crashStep;
                }
            }
            else
            {
                if (m_PatrolController && m_PatrolController.m_ShouldResetCombat)
                {
                    m_PatrolController.m_ShouldResetCombat = false;
                    m_PrimaryTarget = null;
                    m_PatrolAggroTarget = null;
                    m_HasLineOfSight = false;
                    m_ActiveHostiles.Clear();
                }

            m_AITickCounter++;
            if (m_AITickCounter >= AI_UPDATE_INTERVAL)
            {
              m_AITickCounter = 0;
              ProcessWeaponSystems(); 
            }
            }

            if (!m_IsCrashed)
            {
                m_MarkerTickCounter++;
                if (m_MarkerTickCounter >= MARKER_UPDATE_TICKS)
                {
                    m_MarkerTickCounter = 0;
                    UpdateMapMarkers();
                }
                
                if (m_CurrentVisualDamageLevel > GameConstants.STATE_PRISTINE)
                {
                    m_DamageSyncTickCounter++;
                    if (m_DamageSyncTickCounter >= DAMAGE_SYNC_TICKS)
                    {
                        m_DamageSyncTickCounter = 0;
                        SendDamageLevelToClients(m_CurrentVisualDamageLevel);
                    }
                }
            }
        }
        #endif
    }

    #ifdef SERVER
    void SendVelSyncIfDue(float dt, bool force = false)
    {

        float syncNow = GetGame().GetTickTime();
        float sinceSync = syncNow - m_LastVelSyncTime;
        if (!force && m_LastVelSyncTime >= 0 && sinceSync < VEL_SYNC_MIN_INTERVAL) return;

        vector syncVel = "0 0 0";
        if (m_IsCrashing)
        {
            syncVel = m_CrashVelocity;
        }
        else if (m_PatrolController)
        {
            syncVel = m_PatrolController.m_CurrentDirection * m_PatrolController.m_CurrentSpeed;
            syncVel[1] = m_PatrolController.m_VerticalVelocity;
        }

        float syncSpeed = syncVel.Length();
        float speedChange = Math.AbsFloat(syncSpeed - m_LastSentSpeed);
        float verticalChange = Math.AbsFloat(syncVel[1] - m_LastSentVertical);
        bool changed = (speedChange > VEL_SYNC_SPEED_STEP || verticalChange > VEL_SYNC_VERTICAL_STEP);
        bool heartbeat = (m_LastVelSyncTime < 0 || sinceSync >= VEL_SYNC_INTERVAL);
        if (!force && !changed && !heartbeat) return;

        m_LastVelSyncTime = syncNow;
        m_LastSentSpeed = syncSpeed;
        m_LastSentVertical = syncVel[1];

        int syncFlags = 0;
        BS_DifficultyConfig syncDiff = GetDiff();
        if (syncDiff && syncDiff.ClientSmoothMovement)
            syncFlags = syncFlags | 1;

        Param2<vector, int> syncData = new Param2<vector, int>(syncVel, syncFlags);
        array<Man> syncPlayers = new array<Man>();
        GetGame().GetPlayers(syncPlayers);
        vector heliPos = GetPosition();

        foreach (Man syncMan : syncPlayers)
        {
            if (!syncMan) continue;
            PlayerIdentity syncIdentity = syncMan.GetIdentity();
            if (!syncIdentity) continue;
            float syncDist = vector.Distance(syncMan.GetPosition(), heliPos);
            if (syncDist > VEL_SYNC_DISTANCE) continue;
            GetGame().RPCSingleParam(this, BS_RPCIds.HELI_VEL_SYNC, syncData, force, syncIdentity);
        }
    }
    #endif

    void UpdateMapMarkers()
    {
        BS_HeliConfig mCfg = GetConfig();
        if (mCfg && mCfg.MarkerSettings && !mCfg.MarkerSettings.EnableHeliMarker)
            return;

        bool do3D = true;
        bool do2D = true;
        if (mCfg && mCfg.MarkerSettings)
        {
            do3D = mCfg.MarkerSettings.Enable3DMarker;
            do2D = mCfg.MarkerSettings.Enable2DMarker;
        }

        vector markerPos = GetPosition();
        if (m_MarkerPlaced)
        {
            float markerMove = vector.Distance(markerPos, m_LastMarkerPos);
            if (markerMove < MARKER_MIN_MOVE)
                return;
        }
        m_LastMarkerPos = markerPos;
        m_MarkerPlaced = true;

        #ifdef EXPANSIONMODNAVIGATION
            if (do2D || do3D)
                UpdateExpansionMarker();
        #endif

        #ifdef LBmaster_Groups
            if (do2D || do3D)
                UpdateLBMarker();
        #endif
    }

    string BS_HeliMarkerNameColor(out int color)
    {
        int defColor = ARGB(255, 255, 0, 0);
        color = defColor;
        string name = "#STR_BS_Heli_Marker";
        BS_HeliConfig nameCfg = GetConfig();
        if (!nameCfg || !nameCfg.MarkerSettings) return name;
        BS_MarkerSettings ms = nameCfg.MarkerSettings;
        if (ms.HeliMarkerName != "") name = ms.HeliMarkerName;
        color = BS_HeliColor.Parse(ms.HeliMarkerColor, defColor);
        if (!ms.ShowHealthOnMarker) return name;
        float healthRatio = GetHealth01("", "Health");
        if (m_CustomMaxHealth > 0) healthRatio = m_CustomCurrentHealth / m_CustomMaxHealth;
        return BS_HeliMarkerText.WithHealth(name, healthRatio);
    }

    void RemoveMapMarkers()
    {
        #ifdef EXPANSIONMODNAVIGATION
            RemoveExpansionMarker();
        #endif

        #ifdef LBmaster_Groups
            RemoveLBMarker();
        #endif
    }

    #ifdef EXPANSIONMODNAVIGATION
    void UpdateExpansionMarker()
    {
        ExpansionMarkerModule markerModule;
        if (!CF_Modules<ExpansionMarkerModule>.Get(markerModule)) return;

        vector pos = GetPosition();
        
        if (m_ExpansionMarkerUID != "")
        {
            markerModule.RemoveServerMarker(m_ExpansionMarkerUID);
            m_ExpansionMarkerUID = "";
        }

        int color;
        string markerName = BS_HeliMarkerNameColor(color);
        string expIcon = "Helicopter BS";

        bool is3D = true;
        BS_HeliConfig eCfg = GetConfig();
        if (eCfg && eCfg.MarkerSettings)
        {
            is3D = eCfg.MarkerSettings.Enable3DMarker;
            if (eCfg.MarkerSettings.ExpansionMarkerIcon != "") expIcon = eCfg.MarkerSettings.ExpansionMarkerIcon;
        }

        ExpansionMarkerData newMarker = markerModule.CreateServerMarker(markerName, expIcon, pos, color, is3D);
        
        if (newMarker)
        {
            m_ExpansionMarkerUID = newMarker.GetUID();
        }
    }

    void RemoveExpansionMarker()
    {
        if (m_ExpansionMarkerUID != "")
        {
            ExpansionMarkerModule markerModule;
            if (CF_Modules<ExpansionMarkerModule>.Get(markerModule))
            {
                markerModule.RemoveServerMarker(m_ExpansionMarkerUID);
                BS_Patrol_Log.GetInstance().Info("Expansion Marker removed.");
            }
            m_ExpansionMarkerUID = "";
        }
    }
    #endif

    #ifdef LBmaster_Groups
    void UpdateLBMarker()
    {
        if (!LBStaticMarkerManager || !LBStaticMarkerManager.Get) return;

        if (m_LBMarkerUID != 0)
        {
            LBStaticMarkerManager.Get.RemoveServerMarker(m_LBMarkerUID);
            m_LBMarkerUID = 0;
        }

        int color;
        string markerName = BS_HeliMarkerNameColor(color);
        string iconPath = "BS_Event_Logo/Logo/BS_Patrol.paa";

        bool is2D = true;
        bool is3D = true;
        BS_HeliConfig lCfg = GetConfig();
        if (lCfg && lCfg.MarkerSettings)
        {
            is2D = lCfg.MarkerSettings.Enable2DMarker;
            is3D = lCfg.MarkerSettings.Enable3DMarker;
            if (lCfg.MarkerSettings.LBMarkerIcon != "") iconPath = lCfg.MarkerSettings.LBMarkerIcon;
        }
        
        LBServerMarker marker = LBStaticMarkerManager.Get.AddTempServerMarker(markerName, GetPosition(), iconPath, color, true, is3D, is2D);
        
        if (marker)
        {
            m_LBMarkerUID = marker.uid;
        }
    }

    void RemoveLBMarker()
    {
        if (m_LBMarkerUID != 0 && LBStaticMarkerManager && LBStaticMarkerManager.Get)
        {
            LBStaticMarkerManager.Get.RemoveServerMarker(m_LBMarkerUID);
            m_LBMarkerUID = 0;
            BS_Patrol_Log.GetInstance().Info("LB Marker removed.");
        }
    }
    #endif

    protected int m_BS_ClientRotors = -1;

    bool BS_HasClientRotors()
    {
        if (m_BS_ClientRotors < 0)
        {
            m_BS_ClientRotors = 0;
            string clientRotors = BS_GetHeliConfigText("bsClientRotors");
            if (clientRotors != "" && clientRotors != "0")
                m_BS_ClientRotors = 1;
        }
        return (m_BS_ClientRotors == 1);
    }

    void AnimateRotors(float dt)
    {
        m_RotorAnimationPosition = m_RotorAnimationPosition + m_RotorSpeed * dt;
        if (m_RotorAnimationPosition >= 1.0)
        {
            m_RotorAnimationPosition = m_RotorAnimationPosition - 1.0;
        }

        SetAnimationPhase("rotor_main", m_RotorAnimationPosition);
        SetAnimationPhase("rotor_rear", m_RotorAnimationPosition);
        
        if (m_RotorSpeed > 2.0)
        {
            SetAnimationPhase("rotor_main_static", 1.0);
            SetAnimationPhase("rotor_main_blur", 0.0);
        }
        else
        {
            SetAnimationPhase("rotor_main_static", 0.0);
            SetAnimationPhase("rotor_main_blur", 1.0);
        }
    }

    protected float m_ClientDebugTimer = 0.0;
    protected vector m_LastDebugPos = "0 0 0";
    
    void UpdateBanking(float dt)
    {
        vector currentPos = GetPosition();
        vector currentOri = GetOrientation();
        float currentYaw = currentOri[0];
        
        float yawChange = currentYaw - m_LastClientYaw;
        while (yawChange > 180) yawChange = yawChange - 360;
        while (yawChange < -180) yawChange = yawChange + 360;
        
        m_LastClientYaw = currentYaw;
        
        float instantTurnRate = 0;
        if (dt > 0.001) instantTurnRate = yawChange / dt;
        
        m_SmoothedTurnRate = m_SmoothedTurnRate + (instantTurnRate - m_SmoothedTurnRate) * dt * 2.0;
        
        float targetBank = m_SmoothedTurnRate * 1.5;
        if (targetBank > 30.0) targetBank = 30.0;
        if (targetBank < -30.0) targetBank = -30.0;
        if (targetBank > -2.0 && targetBank < 2.0) targetBank = 0.0;
        
        m_VisualBank = m_VisualBank + (targetBank - m_VisualBank) * dt * 2.0;
        
        m_ClientDebugTimer = m_ClientDebugTimer + dt;
        if (m_ClientDebugTimer >= 0.5)
        {
            m_ClientDebugTimer = 0.0;
            m_LastDebugPos = currentPos;
        }
    }

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (rpc_type == BS_RPCIds.FLARE_FX && !GetGame().IsServer())
        {
            Param2<vector, vector> flareParams;
            if (!ctx.Read(flareParams)) return;
            PlayFlareFX(flareParams.param1, flareParams.param2);
        }

        if (rpc_type == BS_RPCIds.HELI_VEL_SYNC && !GetGame().IsServer())
        {
            Param2<vector, int> velParams = new Param2<vector, int>(vector.Zero, 0);
            if (!ctx.Read(velParams)) return;
            m_SyncVel = velParams.param1;
            m_SyncVelValid = true;
            int velFlags = velParams.param2;
            bool smoothWanted = ((velFlags & 1) != 0);
            if (smoothWanted != m_SmoothMovement)
            {
                m_NetInit = false;
                m_HasPreviousPos = false;
                m_SmoothMovement = smoothWanted;
            }
        }

        if (rpc_type == BS_RPCIds.FLARE_BURST && !GetGame().IsServer())
        {
            Param4<vector, int, float, float> burstParams = new Param4<vector, int, float, float>(vector.Zero, 0, 0.0, 0.0);
            if (!ctx.Read(burstParams)) return;
            BS_FlareManager flares = BS_FlareManager.Get();
            flares.StartBurst(this, burstParams.param1, burstParams.param2, burstParams.param3, burstParams.param4);
        }
        
        if (rpc_type == BS_RPCIds.TAIL_BREAK && !GetGame().IsServer())
        {
            vector tbV0;
            vector tbW;
            float tbLandT;
            vector tbRestPos;
            vector tbRestOri;
            bool tbWater;
            if (!ctx.Read(tbV0)) return;
            if (!ctx.Read(tbW)) return;
            if (!ctx.Read(tbLandT)) return;
            if (!ctx.Read(tbRestPos)) return;
            if (!ctx.Read(tbRestOri)) return;
            if (!ctx.Read(tbWater)) return;
            BS_OnTailBreakClient(tbV0, tbW, tbLandT, tbRestPos, tbRestOri, tbWater);
        }

        if (rpc_type == BS_RPCIds.TURRET_SOUND && !GetGame().IsServer())
        {
            Param2<vector, vector> param;
            if (!ctx.Read(param)) return;
            OnGunFiredFX(param.param1, param.param2);
        }
        
        if (rpc_type == BS_RPCIds.CRASH_SOUND && !GetGame().IsServer())
        {
            Param1<vector> crashParam;
            if (!ctx.Read(crashParam)) return;
            OnCrashImpactFX(crashParam.param1);
        }

        if (rpc_type == BS_RPCIds.SMOKE_TRAIL && !GetGame().IsServer())
        {
            Param1<vector> trailParams;
            if (ctx.Read(trailParams))
            {
                ParticleManager.GetInstance().PlayInWorld(ParticleList.SMOKING_BARREL_HEAVY, trailParams.param1);
            }
        }
    
        if (rpc_type == BS_RPCIds.BOUNCE_IMPACT && !GetGame().IsServer())
        {
            Param1<vector> bounceParams;
            if (ctx.Read(bounceParams))
            {
                EffectSound impactSound = SEffectManager.PlaySound("offroad_hit_heavy_SoundSet", bounceParams.param1);
                if (impactSound) impactSound.SetSoundAutodestroy(true);
                
                ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_DIRT_RICOCHET, bounceParams.param1);
            }
        }
    
        if (rpc_type == BS_RPCIds.FINAL_EXPLOSION && !GetGame().IsServer())
        {
            Param1<vector> explosionParams;
            if (ctx.Read(explosionParams))
            {
                vector pos = explosionParams.param1;
                
                ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_LANDMINE, pos);
                ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_TEST, pos + Vector(0, 2, 0));
                
                EffectSound boom = SEffectManager.PlaySound("Plastic_Explosion_SoundSet", pos);
                if (boom) 
                {
                    boom.SetSoundAutodestroy(true);
                    boom.SetSoundVolume(3.0);
                }
                
                GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(PlayDelayedExplosionSound, 200, false, pos);
            }
        }
    
        if (rpc_type == BS_RPCIds.CRASH_START && !GetGame().IsServer())
        {
            Param3<vector, int, float> crashStartParams;
            if (ctx.Read(crashStartParams))
            {
                StopFlyingSoundClient();
                m_IsCrashing = true;
                if (!m_SmoothMovement)
                    m_HasPreviousPos = false;
                CreateCrashEffects();
            }
        }

        if (rpc_type == BS_RPCIds.DAMAGE_LEVEL_SYNC && !GetGame().IsServer())
        {
            Param1<int> damageLevelParams;
            if (ctx.Read(damageLevelParams))
            {
                m_LastVisualDamageLevel = -1;
                ApplyDamageVisuals(damageLevelParams.param1);
            }
        }
    }

    void PlayDelayedExplosionSound(vector pos)
    {
        EffectSound echo = SEffectManager.PlaySound("Grenade_explosion_SoundSet", pos);
        if (echo)
        {
            echo.SetSoundAutodestroy(true);
            echo.SetSoundVolume(2.0);
        }
    }

#ifdef SERVER
    SurvivorBase ScanForPatrolTarget(float range)
    {
        vector heliPos = GetPosition();
        vector heliForward = GetDirection();
        heliForward[1] = 0;
        heliForward.Normalize();
        
        float bestDist = range + 1.0;
        SurvivorBase bestTarget;
        BS_HeliConfig scanCfg = GetConfig();

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            SurvivorBase sb = SurvivorBase.Cast(man);
            if (!sb || !sb.IsAlive() || sb.IsUnconscious()) continue;
            if (BS_HeliTargets.Ignore(scanCfg, sb)) continue;

            float dist = vector.Distance(heliPos, sb.GetPosition());
            if (dist > range) continue;
            
            vector toPlayer = sb.GetPosition() - heliPos;
            toPlayer[1] = 0;
            toPlayer.Normalize();
            
            float dotProduct = heliForward[0] * toPlayer[0] + heliForward[2] * toPlayer[2];
            
            if (dotProduct < -0.17) continue; 
            
            vector muzzleWS = BS_GetMuzzleWorld();
            vector playerPos = sb.GetPosition() + Vector(0, TARGET_AIM_OFFSET, 0);
            
            vector hitPos, hitNorm;
            int hitComp;
            set<Object> hitObjects = new set<Object>();
            
            bool hasLOS = false;
            if (DayZPhysics.RaycastRV(muzzleWS + (playerPos - muzzleWS).Normalized() * 1.5, playerPos, hitPos, hitNorm, hitComp, hitObjects, null, this, false, false, ObjIntersectFire))
            {
                if (hitObjects.Count() > 0)
                {
                    Object hitObj = hitObjects[0];
                    if (hitObj == sb || hitObj.IsInherited(SurvivorBase) || hitObj.IsInherited(CarScript))
                        hasLOS = true;
                }
            }
            else
            {
                hasLOS = true;
            }
            
            if (dist < 15.0) hasLOS = true;
            
            if (hasLOS && dist < bestDist)
            {
                bestDist = dist;
                bestTarget = sb;
            }
        }
        
        return bestTarget;
    }

void ProcessWeaponSystems()
{
    if (m_BS_Leaving)
    {
        m_PrimaryTarget = null;
        m_HasLineOfSight = false;
        return;
    }
    if (m_IsCrashed || m_IsCrashing || !m_GunReady) return;
    
    BS_DifficultyConfig diff = GetDiff();
    float nowMS = GetGame().GetTime();
    float tickDt = AI_UPDATE_INTERVAL * 0.025;

    float heatDissipationRate = 25.0 * tickDt; 
    m_WeaponHeat = Math.Max(0.0, m_WeaponHeat - heatDissipationRate);

    if (m_WeaponOverheated)
    {
        if (m_WeaponHeat < 15.0) 
        {
            m_WeaponOverheated = false;
        }
    }

    float sinceDamageSeconds = (nowMS - m_LastHitTime) / 1000.0;
    m_TakingDamageRecently = (m_LastHitTime > 0 && sinceDamageSeconds < 4.0);

    if (m_TakingDamageRecently && !m_HasLineOfSight && m_PrimaryTarget)
    {
        m_NoLOSWhileDamagedTimer += tickDt;
    }
    else
    {
        m_NoLOSWhileDamagedTimer = 0.0;
    }

    if (!m_IsSuppressing && m_DamageAccumulator >= diff.SuppressionDamageThreshold)
    {
        int attackerCount = 0;
        if (m_ActiveHostiles) attackerCount = m_ActiveHostiles.Count();
        float requiredNoLOSTime = diff.SuppressionNoLOSTime - ((attackerCount - 1) * diff.SuppressionMultiAttackerBonus);
        if (requiredNoLOSTime < 1.5) requiredNoLOSTime = 1.5; 

        if (m_NoLOSWhileDamagedTimer >= requiredNoLOSTime)
        {
            vector suppressPos = "0 0 0";
            SurvivorBase lastDealer = FindClosestAttacker();
            if (lastDealer)
                suppressPos = lastDealer.GetPosition();
            else if (m_PrimaryTarget)
                suppressPos = m_PrimaryTarget.GetPosition();

            if (suppressPos != vector.Zero)
            {
                m_IsSuppressing = true;
                m_SuppressionTimer = 0.0;
                m_SuppressionTargetPos = suppressPos;
                m_DamageAccumulator = 0;
                m_NoLOSWhileDamagedTimer = 0.0;
                
                BS_Patrol_Log.GetInstance().Info("SUPPRESSION ACTIVATED. Attackers: " + attackerCount + ". Target position: " + m_SuppressionTargetPos);
            }
        }
    }

    if (m_IsSuppressing)
    {
        m_SuppressionTimer += tickDt;

        if (m_SuppressionTimer >= diff.SuppressionDuration)
        {
            m_IsSuppressing = false; 
            BS_Patrol_Log.GetInstance().Info("Suppression mode ended. Searching for targets...");
            return;
        }

        vector jitter;
        jitter[0] = Math.RandomFloat(-diff.SuppressionSpread, diff.SuppressionSpread);
        jitter[2] = Math.RandomFloat(-diff.SuppressionSpread, diff.SuppressionSpread);
        jitter[1] = Math.RandomFloat(-1.0, 2.0); 
        
        vector areaTarget = m_SuppressionTargetPos + jitter;
        
        vector muzzleWS_supp = BS_GetMuzzleWorld(); 
        AimCannonAtTarget(muzzleWS_supp, areaTarget);

        m_MissileTimer += 0.1; 
        
        if (m_MissileTimer >= diff.SuppressionMissileRate)
        {
            FireMissile(areaTarget);
            m_MissileTimer = 0.0;
            
            ExecuteSimulatedShot(muzzleWS_supp, areaTarget - muzzleWS_supp);
        }
        
        return; 
    }

    if (m_LastHitTimeMS > 0) 
    {
        float sinceDamage = (nowMS - m_LastHitTimeMS) / 1000.0;
        
        if (m_PrimaryTarget && (!m_PrimaryTarget.IsAlive() || m_PrimaryTarget.IsUnconscious()))
        {
            m_PrimaryTarget = null;
            m_HasLineOfSight = false;
            
            #ifdef SERVER
            if (!SelectNextTarget())
            {
                m_ActiveHostiles.Clear();
                BS_Patrol_Log.GetInstance().Info("[BS_Apache] All hostiles eliminated. Returning to patrol.");
            }
            #endif
        }
        
        bool inActiveCombat = false;
        #ifdef SERVER
        if (m_PatrolController && m_PatrolController.m_InCombat)
            inActiveCombat = true;
        #endif
        
        float hostileForgetTimeout = Math.Max(diff.TurretAttackCooldown, 45.0);
        if (!inActiveCombat && sinceDamage > hostileForgetTimeout && m_ActiveHostiles.Count() > 0)
        {
            m_ActiveHostiles.Clear();
            BS_Patrol_Log.GetInstance().Info("[BS_Apache] No damage for " + hostileForgetTimeout + "s and not in combat. Clearing hostiles.");
        }
    }
    
    ref BS_HeliConfig patrolCfg = GetConfig();
    if (patrolCfg && patrolCfg.HelicopterSettings.AggroWhilePatrolling && !m_PrimaryTarget)
    {
        m_PatrolScanTimer += tickDt;
        if (m_PatrolScanTimer >= 0.5) 
        {
            m_PatrolScanTimer = 0.0;
            SurvivorBase patrolTarget = ScanForPatrolTarget(patrolCfg.HelicopterSettings.PatrolAggroRange);
            
            if (patrolTarget)
            {
                m_PatrolAggroTarget = patrolTarget;
                m_PrimaryTarget = patrolTarget;
                m_TimeSinceTargetAcquired = nowMS;
                m_TargetFocusTime = nowMS;
                
                if (patrolCfg.HelicopterSettings.PatrolAggroPursue)
                {
                    EngagePatrolTarget(patrolTarget);
                    
                    if (m_ActiveHostiles.Find(patrolTarget) == -1) 
                        m_ActiveHostiles.Insert(patrolTarget);
                    
                    BS_Patrol_Log.GetInstance().Info("PATROL AGGRO: Target detected at " + vector.Distance(GetPosition(), patrolTarget.GetPosition()) + "m");
                }
            }
        }
    }
    
    if (m_PatrolAggroTarget)
    {
        bool shouldClearPatrolTarget = false;
        
        if (!m_PatrolAggroTarget.IsAlive() || m_PatrolAggroTarget.IsUnconscious())
            shouldClearPatrolTarget = true;
        
        if (!shouldClearPatrolTarget && patrolCfg)
        {
            float distToPatrolTarget = vector.Distance(GetPosition(), m_PatrolAggroTarget.GetPosition());
            if (distToPatrolTarget > patrolCfg.HelicopterSettings.PatrolAggroRange * 1.2) 
                shouldClearPatrolTarget = true;
        }
        
        if (shouldClearPatrolTarget)
        {
            if (m_PrimaryTarget == m_PatrolAggroTarget && m_ActiveHostiles.Find(m_PatrolAggroTarget) == -1)
            {
                m_PrimaryTarget = null;
                m_HasLineOfSight = false;
            }
            m_PatrolAggroTarget = null;
        }
    }
    
    SurvivorBase target;
    if (m_PrimaryTarget && m_PrimaryTarget.IsAlive()) 
        target = m_PrimaryTarget;
    else 
        return;

    #ifdef SERVER
    if (patrolCfg && patrolCfg.IsInSafeZone(target.GetPosition()))
    {
        BS_Patrol_Log.GetInstance().Info("[BS_Apache] Target entered safe zone. Disengaging.");
        if (m_PatrolController)
            m_PatrolController.m_ShouldResetCombat = true;
        return;
    }
    #endif

    if (m_PatrolAggroTarget && target == m_PatrolAggroTarget)
    {
        vector heliForwardCheck = GetDirection();
        heliForwardCheck[1] = 0;
        heliForwardCheck.Normalize();
        
        vector toTargetCheck = target.GetPosition() - GetPosition();
        toTargetCheck[1] = 0;
        toTargetCheck.Normalize();
        
        float arcDot = heliForwardCheck[0] * toTargetCheck[0] + heliForwardCheck[2] * toTargetCheck[2];
        
        if (arcDot < -0.17)
        {
            m_PrimaryTarget = null;
            m_PatrolAggroTarget = null;
            m_HasLineOfSight = false;
            return;
        }
    }

    vector muzzleWS_Normal = BS_GetMuzzleWorld();
    float dist = vector.Distance(muzzleWS_Normal, target.GetPosition());
    if (dist > diff.TurretMaxRange) { m_HasLineOfSight = false; return; }

    vector targetVel = GetVelocity(target);
    
    if (targetVel.Length() < 1.0)
    {
        EntityAI parent = EntityAI.Cast(target.GetParent());
        if (parent)
        {
            vector parentVel = GetVelocity(parent);
            if (parentVel.Length() > targetVel.Length())
                targetVel = parentVel;
        }
    }
    
    float bulletSpeed = 500.0;
    
    vector predictedPos = target.GetPosition();
    for (int leadIter = 0; leadIter < 2; leadIter++)
    {
        float iterDist = vector.Distance(muzzleWS_Normal, predictedPos);
        float timeToHit = iterDist / bulletSpeed;
        predictedPos = target.GetPosition() + (targetVel * timeToHit);
    }
    
    vector targetWS = predictedPos + Vector(0, TARGET_AIM_OFFSET, 0);

    AimCannonAtTarget(muzzleWS_Normal, targetWS);
    
    vector dirToTarget = targetWS - muzzleWS_Normal;
    dirToTarget.Normalize();

    vector startRay = muzzleWS_Normal + (dirToTarget * 1.5); 
    vector endRay = target.GetPosition() + Vector(0, TARGET_AIM_OFFSET, 0);
    
    vector hitPos, hitNorm; 
    int hitComp; 
    set<Object> hitObjects = new set<Object>();
    
    m_HasLineOfSight = false; 
    if (DayZPhysics.RaycastRV(startRay, endRay, hitPos, hitNorm, hitComp, hitObjects, null, this, false, false, ObjIntersectFire)) 
    {
        if (hitObjects.Count() > 0) 
        {
            Object hitObj = hitObjects[0];
            if (hitObj == target || hitObj.IsInherited(SurvivorBase) || hitObj.IsInherited(CarScript)) 
                m_HasLineOfSight = true;
        }
    } 
    else m_HasLineOfSight = true; 

    if (dist < 15.0) m_HasLineOfSight = true;

    m_MissileTimer = m_MissileTimer + 0.1;
    if (!m_HasLineOfSight) return;

    bool isPatrolAggroOnly = (m_PatrolAggroTarget && target == m_PatrolAggroTarget && m_ActiveHostiles.Find(target) == -1);

    if (!isPatrolAggroOnly && dist > diff.MissileMinRange && dist < diff.MissileMaxRange && m_MissileTimer >= diff.MissileCooldown)
    {
        vector heliDir = GetDirection(); 
        vector dirToPlayer = target.GetPosition() - GetPosition();
        dirToPlayer.Normalize();
        float angleCheck = vector.Dot(heliDir, dirToPlayer);
        
        if (angleCheck > diff.MissileAngleThreshold)
        {
            FireMissileAt(targetWS, target); 
            m_MissileTimer = 0.0;  

            float currentHealth = GetHealth("", "Health");
            float maxHealth = GetMaxHealth("", "Health");
            if (m_CustomMaxHealth > 0) 
            { 
                maxHealth = m_CustomMaxHealth; 
                currentHealth = m_CustomCurrentHealth; 
            }
            float healthPerc = 1.0;
            if (maxHealth > 0) healthPerc = currentHealth / maxHealth;

            int enemyCount = 0;
            if (m_ActiveHostiles) enemyCount = m_ActiveHostiles.Count();
            bool isDesperate = (healthPerc < 0.40) || (enemyCount > 2);

            if (isDesperate && Math.RandomFloat01() < diff.MissileDoubleShotChance)
            {
                FireMissileAt(targetWS, target);
            }
        }
    }

    if (m_WeaponOverheated) return; 

    float sinceShot = (nowMS - m_LastFireTimeMS) / 1000.0;
    
    if (sinceShot >= diff.TurretFireCooldown && BS_BurstReady(nowMS))
    {
        ExecuteSimulatedShot(muzzleWS_Normal, targetWS - muzzleWS_Normal);
        BS_BurstShot(nowMS);
        m_LastFireTimeMS = nowMS;
        
        // Cada disparo añade una cantidad aleatoria de calor
        float heatPerShot = Math.RandomFloat(5.0, 9.0); 
        m_WeaponHeat += heatPerShot;
        
        // Si superamos los 100 grados de temperatura, el arma se bloquea (Overheat)
        if (m_WeaponHeat > 100.0)
        {
            m_WeaponOverheated = true;
        }
      }
    }
    #endif

    void AimCannonAtTarget(vector muzzleWS, vector targetWS) 
    {
        vector toTarget = targetWS - muzzleWS;
        vector angles = toTarget.VectorToAngles();
        vector heliAngles = GetOrientation();
        
        float yawDiff = angles[0] - heliAngles[0];
        
        while (yawDiff > 180.0) yawDiff -= 360.0;
        while (yawDiff < -180.0) yawDiff += 360.0;
        
        float finalYaw = Math.Clamp(yawDiff, -170.0, 170.0);
        SetAnimationPhase(ANIM_GUN_YAW, GetNormalizedPhase(finalYaw, -170.0, 170.0));
        m_GunYawDeg = finalYaw;

        float distFlat = vector.Distance(Vector(muzzleWS[0], 0, muzzleWS[2]), Vector(targetWS[0], 0, targetWS[2]));
        float pitchDeg = Math.Atan2(toTarget[1], distFlat) * Math.RAD2DEG;
        
        float finalPitch = Math.Clamp(pitchDeg, -45.0, 45.0);
        SetAnimationPhase(ANIM_GUN_PITCH, (finalPitch - 45.0) / (-45.0 - 45.0));
        m_GunPitchDeg = finalPitch;
    }

    void ExecuteSimulatedShot(vector originWS, vector dirWS, string ammoType = "") 
    {
        BS_DifficultyConfig diff = GetDiff();
        
        if (ammoType == "")
            ammoType = diff.TurretAmmoType;

        dirWS.Normalize(); 
        vector muzzleWS = BS_GetMuzzleWorld();
        float maxRange = diff.TurretMaxRange; 
        vector rayEndPoint = muzzleWS + (dirWS * maxRange);
        
        vector hitPos, hitNorm; 
        int hitComp; 
        set<Object> hitObjects = new set<Object>();
        
        bool isHitSuccessful = (Math.RandomFloat01() <= diff.TurretAccuracy); 
        if (!isHitSuccessful) 
        {
            float randomOffsetX = Math.RandomFloatInclusive(-diff.TurretMissAngle, diff.TurretMissAngle);
            float randomOffsetY = Math.RandomFloatInclusive(-diff.TurretMissAngle, diff.TurretMissAngle);
            float randomOffsetZ = Math.RandomFloatInclusive(-diff.TurretMissAngle, diff.TurretMissAngle);
            
            vector deviation = Vector(randomOffsetX, randomOffsetY, randomOffsetZ);
            rayEndPoint = rayEndPoint + (deviation * maxRange * 0.05);
        }

        bool didHit = DayZPhysics.RaycastRV(muzzleWS, rayEndPoint, hitPos, hitNorm, hitComp, hitObjects, null, this, false, false, ObjIntersectFire);
        
        if (didHit) 
        {
            rayEndPoint = hitPos;
            if (hitObjects.Count() > 0) 
            {
                SurvivorBase sb = SurvivorBase.Cast(EntityAI.Cast(hitObjects[0]));
                if (sb) 
                    GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(InflictProjectileDamage, 300, false, sb, hitComp, hitPos, ammoType);
            }
        }
        
        BS_RPCHelper.SendToNearby_Param2Vec(this, BS_RPCIds.TURRET_SOUND, muzzleWS, rayEndPoint, muzzleWS, RPC_MAX_DISTANCE, false);
    }

    void InflictProjectileDamage(Object obj, int hitComp, vector hitPos, string ammoType) 
    {
        SurvivorBase sb = SurvivorBase.Cast(obj);
        if (sb && sb.IsAlive()) 
            sb.ProcessDirectDamage(DT_FIRE_ARM, this, "Torso", ammoType, hitPos, 0.5, ProcessDirectDamageFlags.ALL_TRANSFER);
    }

    void FireMissile(vector targetPos)
    {
        FireMissileAt(targetPos, null);
    }

    void FireMissileAt(vector targetPos, EntityAI target)
    {
        BS_DifficultyConfig diff = GetDiff();
        
        vector offset = BS_GetMissileLaunchLocal(m_FireLeftPod);

        vector spawnPos = ModelToWorld(offset);
        vector dir = targetPos - spawnPos;
        dir.Normalize();

        Object missileObj = GetGame().CreateObject("BS_Missile_Entity", spawnPos);
        BS_Missile_Entity missile = BS_Missile_Entity.Cast(missileObj);
        
        if (missile)
        {
            missile.BS_SetLauncher(this);
            if (target)
                missile.FiredAt(dir, diff.MissileSpeed, target);
            else
                missile.Fired(dir, diff.MissileSpeed);
            m_FireLeftPod = !m_FireLeftPod;
        }
    }

    static const float FLARE_RPC_DISTANCE = 1500.0;

    void DeployFlares()
    {
        if (m_IsCrashing || m_IsCrashed) return;

        BS_DifficultyConfig diff = GetDiff();

        if (GetGame().GetTime() - m_FlareTimer < (diff.FlareCooldown * 1000)) return;
        m_FlareTimer = GetGame().GetTime();

        vector heliVel = "0 0 0";
        #ifdef SERVER
        if (m_PatrolController)
        {
            heliVel = m_PatrolController.m_CurrentDirection * m_PatrolController.m_CurrentSpeed;
            heliVel[1] = m_PatrolController.m_VerticalVelocity;
        }
        #endif

        int pairs = diff.FlareBurstPairs;
        if (pairs < 1) pairs = 1;
        if (pairs > 12) pairs = 12;
        float interval = Math.Clamp(diff.FlarePairInterval, 0.1, 1.5);
        float burnSeconds = Math.Clamp(diff.FlareBurnSeconds, 1.0, 10.0);

        if (!GetGame().IsMultiplayer())
        {
            BS_FlareManager offlineFlares = BS_FlareManager.Get();
            offlineFlares.StartBurst(this, heliVel, pairs, interval, burnSeconds);
            return;
        }

        Param4<vector, int, float, float> burstData = new Param4<vector, int, float, float>(heliVel, pairs, interval, burnSeconds);
        array<Man> flarePlayers = new array<Man>();
        GetGame().GetPlayers(flarePlayers);
        vector heliPos = GetPosition();

        foreach (Man flareMan : flarePlayers)
        {
            if (!flareMan) continue;
            PlayerIdentity flareIdentity = flareMan.GetIdentity();
            if (!flareIdentity) continue;
            float flareDist = vector.Distance(flareMan.GetPosition(), heliPos);
            if (flareDist > FLARE_RPC_DISTANCE) continue;
            GetGame().RPCSingleParam(this, BS_RPCIds.FLARE_BURST, burstData, true, flareIdentity);
        }
    }

    #ifdef SERVER
    void ApplyCrashPhysics(float dt)
    {
        m_CrashTotalTime += dt;
        m_CrashPhaseTimer += dt;

        if (m_BS_TailBreakPending && m_CrashTotalTime >= TAIL_BREAK_DELAY && !m_CrashImpactPending && !m_CrashSliding)
            BS_BreakTail();

        if (m_CrashImpactPending)
        {
            m_CrashImpactTimer -= dt;
            if (m_CrashImpactTimer <= 0)
            {
                m_CrashImpactPending = false;
                FinalCrash(m_CrashImpactPos, m_CrashImpactGround);
            }
            return;
        }

        vector pos = GetPosition();
        float groundHeight = BS_CrashFloorY(pos[0], pos[2]);
        float heightAGL = pos[1] - m_CrashBottom - groundHeight;

        if (m_CrashSliding)
        {
            BS_CrashSlideStep(dt, pos);
            return;
        }

        BS_CrashFallStep(dt);

        vector newPos = pos + (m_CrashVelocity * dt);
        float crashEdgeDist = BS_MapBounds.EdgeDistance(pos);
        float crashEdge = Math.Min(BS_MapBounds.GetEdge(), crashEdgeDist);
        vector crashSafe = BS_MapBounds.Clamp(newPos, crashEdge);
        bool crashOutX = (crashSafe[0] != newPos[0]);
        bool crashOutZ = (crashSafe[2] != newPos[2]);
        if (crashOutX || crashOutZ)
        {
            float crashVX = m_CrashVelocity[0];
            float crashVY = m_CrashVelocity[1];
            float crashVZ = m_CrashVelocity[2];
            if (crashOutX) crashVX = 0.0;
            if (crashOutZ) crashVZ = 0.0;
            m_CrashVelocity = Vector(crashVX, crashVY, crashVZ);
            newPos = crashSafe;
        }
        float newGroundHeight = BS_CrashFloorY(newPos[0], newPos[2]);
        if (newPos[1] - m_CrashBottom <= newGroundHeight)
        {
            BS_CrashTouchdown(newPos, newGroundHeight);
            return;
        }

        // Escape check: si ya estamos dentro de un solido, crash inmediato
        {
            vector escapeRayEnd = pos + Vector(0, 2.5, 0);
            set<Object> escapeHits = new set<Object>();
            vector eHitPos, eHitNorm;
            int eHitComp;
            if (DayZPhysics.RaycastRV(pos, escapeRayEnd, eHitPos, eHitNorm, eHitComp, escapeHits, null, this, false, false, ObjIntersectGeom))
            {
                if (escapeHits.Count() > 0 && escapeHits[0].IsBuilding())
                {
                    BS_Patrol_Log.GetInstance().Warning("[BS_Apache] Heli inside building geometry! Forcing FinalCrash.");
                    BS_CrashBeginFinal(pos, groundHeight);
                    return;
                }
            }
        }

        if (CheckObjectCollision(pos, newPos))
        {
            float keptDown = m_CrashVelocity[1] * 0.85;
            m_CrashVelocity = Vector(0, keptDown, 0);
            m_CrashYawRate = m_CrashYawRate * 0.7;
            CreateCollisionEffects(pos);
            DestroyNearbyTrees(pos, 8.0);
            vector dropStep = Vector(0, keptDown * dt, 0);
            vector dropPos = pos + dropStep;
            if (heightAGL < 1.5 || CheckObjectCollision(pos, dropPos))
            {
                BS_CrashBeginFinal(pos, groundHeight);
                return;
            }
            newPos = dropPos;
        }

        SetPosition(newPos);
        SetOrientation(Vector(m_CrashYaw, m_CrashPitch, m_CrashRoll));
        UpdateCrashEffects(dt, heightAGL);

        if (m_CrashTotalTime - m_LastTrailSpawnTime > 0.15)
        {
            m_LastTrailSpawnTime = m_CrashTotalTime;
            SpawnSmokeTrail(pos);
        }
        SendVelSyncIfDue(dt);
    }
    #endif

    SurvivorBase FindClosestAttacker() 
    {
        float shortestDistSq = 99999999.0; 
        SurvivorBase closestThreat;
        
        for (int i = 0; i < m_ActiveHostiles.Count(); i++)
        {
            SurvivorBase candidate = m_ActiveHostiles[i];
            if (candidate && candidate.IsAlive()) 
            {
                float dSq = vector.DistanceSq(candidate.GetPosition(), GetPosition());
                if (dSq < shortestDistSq) 
                { 
                    shortestDistSq = dSq; 
                    closestThreat = candidate; 
                }
            }
        } 
        return closestThreat;
    }

    #ifdef SERVER
    bool SelectNextTarget()
    {
        if (!m_ActiveHostiles || m_ActiveHostiles.Count() == 0)
            return false;
        
        float closestDist = 9999999.0;
        SurvivorBase nextTarget = null;
        vector heliPos = GetPosition();
        
        for (int i = m_ActiveHostiles.Count() - 1; i >= 0; i--)
        {
            SurvivorBase candidate = m_ActiveHostiles[i];
            if (!candidate || !candidate.IsAlive() || candidate.IsUnconscious())
            {
                m_ActiveHostiles.Remove(i);
                continue;
            }
            float dist = vector.Distance(heliPos, candidate.GetPosition());
            if (dist < closestDist)
            {
                closestDist = dist;
                nextTarget = candidate;
            }
        }
        
        if (nextTarget)
        {
            m_PrimaryTarget = nextTarget;
            m_TimeSinceTargetAcquired = GetGame().GetTime();
            m_TargetFocusTime = GetGame().GetTime();
            m_HasLineOfSight = false;
            EngagePatrolTarget(nextTarget);
            BS_Patrol_Log.GetInstance().Info("[BS_Apache] Target eliminated. Switching to next hostile. Remaining: " + m_ActiveHostiles.Count());
            return true;
        }
        
        return false;
    }
    #endif

    #ifdef SERVER
    void BS_CrashFallStep(float dt)
    {
        float spinK = 1.0 - Math.Pow(Math.EULER, -dt / m_CrashSpinTau);
        float spinGoal = m_SpinDirection * m_CrashSpinTarget;
        m_CrashYawRate = m_CrashYawRate + (spinGoal - m_CrashYawRate) * spinK;

        float velX = m_CrashVelocity[0];
        float velY = m_CrashVelocity[1];
        float velZ = m_CrashVelocity[2];
        float accY = (-m_CrashDescent - velY) / m_CrashDescentTau;
        accY = Math.Clamp(accY, -CRASH_ACC_MAX, CRASH_ACC_MAX);
        velY = velY + accY * dt;
        float horizKeep = Math.Pow(Math.EULER, -dt / m_CrashHorizTau);
        velX = velX * horizKeep;
        velZ = velZ * horizKeep;
        m_CrashVelocity = Vector(velX, velY, velZ);

        float wobT = m_CrashTotalTime;
        float wobPh = m_CrashWobblePhase;
        float wobPitch = 4.0 * Math.Sin(0.9 * wobT + wobPh) + 2.0 * Math.Sin(2.3 * wobT + 2.0 * wobPh);
        float wobRoll = 6.0 * Math.Sin(1.3 * wobT + 3.0 * wobPh) + 2.5 * Math.Sin(3.1 * wobT + wobPh);
        float pitchGoal = m_CrashPitchTarget + wobPitch;
        float rollGoal = m_SpinDirection * m_CrashRollTarget + wobRoll;
        BS_CrashAttitudeStep(dt, pitchGoal, rollGoal, CRASH_ATT_W);

        m_CrashYaw = m_CrashYaw + m_CrashYawRate * dt;
        while (m_CrashYaw >= 360.0) m_CrashYaw = m_CrashYaw - 360.0;
        while (m_CrashYaw < 0.0) m_CrashYaw = m_CrashYaw + 360.0;
    }

    void BS_CrashAttitudeStep(float dt, float pitchGoal, float rollGoal, float omega)
    {
        float damp = 2.0 * CRASH_ATT_Z * omega;
        float omegaSq = omega * omega;
        float pitchAcc = omegaSq * (pitchGoal - m_CrashPitch) - damp * m_CrashPitchRate;
        m_CrashPitchRate = m_CrashPitchRate + pitchAcc * dt;
        float newPitch = m_CrashPitch + m_CrashPitchRate * dt;
        m_CrashPitch = Math.Clamp(newPitch, -CRASH_ATT_MAX, CRASH_ATT_MAX);
        float rollAcc = omegaSq * (rollGoal - m_CrashRoll) - damp * m_CrashRollRate;
        m_CrashRollRate = m_CrashRollRate + rollAcc * dt;
        float newRoll = m_CrashRoll + m_CrashRollRate * dt;
        m_CrashRoll = Math.Clamp(newRoll, -CRASH_ATT_MAX, CRASH_ATT_MAX);
    }

    void BS_CrashTouchdown(vector impactPos, float groundHeight)
    {
        float downSpeed = -m_CrashVelocity[1];
        float impactSpeed = m_CrashVelocity.Length();
        BS_Patrol_Log.GetInstance().Info("Impact! Speed: " + impactSpeed + " m/s, Vertical: " + downSpeed + " m/s");
        vector groundPos = Vector(impactPos[0], groundHeight + m_CrashBottom * 0.85, impactPos[2]);
        BS_RPCHelper.SendToNearby_Param1Vec(this, BS_RPCIds.BOUNCE_IMPACT, groundPos, groundPos, 500.0);
        if (m_CrashOverWater)
        {
            BS_CrashBeginFinal(groundPos, groundHeight);
            return;
        }
        float impactVX = m_CrashVelocity[0];
        float impactVZ = m_CrashVelocity[2];
        float impactForward = Math.Sqrt(impactVX * impactVX + impactVZ * impactVZ) * 0.75;
        bool canSlide = (impactForward > CRASH_SLIDE_MIN_SPEED && impactSpeed < CRASH_SLIDE_MAX_TOTAL);
        if (downSpeed < CRASH_SLIDE_MAX_IMPACT && canSlide)
        {
            m_CrashSliding = true;
            m_CrashSlideTime = 0.0;
            float slideVelX = m_CrashVelocity[0] * 0.75;
            float slideVelZ = m_CrashVelocity[2] * 0.75;
            float hop = Math.Min(2.5, downSpeed * 0.12);
            float slideHoriz = Math.Sqrt(slideVelX * slideVelX + slideVelZ * slideVelZ);
            m_CrashSlideDecel = Math.Max(CRASH_SLIDE_DECEL, slideHoriz / (CRASH_SLIDE_MAX_TIME * 0.8));
            m_CrashVelocity = Vector(slideVelX, hop, slideVelZ);
            m_CrashYawRate = m_CrashYawRate * 0.6;
            SetPosition(groundPos);
            SetOrientation(Vector(m_CrashYaw, m_CrashPitch, m_CrashRoll));
            DestroyNearbyTrees(groundPos, 12.0);
            m_CrashLastTreeTime = m_CrashTotalTime;
            m_CrashLastDustTime = m_CrashTotalTime;
            BS_Patrol_Log.GetInstance().Info("Crash: soft impact, sliding");
            SendVelSyncIfDue(0.025);
            return;
        }
        BS_CrashBeginFinal(groundPos, groundHeight);
    }

    void BS_CrashSlideStep(float dt, vector pos)
    {
        m_CrashSlideTime += dt;
        float velX = m_CrashVelocity[0];
        float velY = m_CrashVelocity[1];
        float velZ = m_CrashVelocity[2];
        float horiz = Math.Sqrt(velX * velX + velZ * velZ);
        float newHoriz = Math.Max(0.0, horiz - m_CrashSlideDecel * dt);
        if (horiz > 0.001)
        {
            float horizScale = newHoriz / horiz;
            velX = velX * horizScale;
            velZ = velZ * horizScale;
        }
        velY = velY - CRASH_GRAVITY * dt;
        vector slideStep = Vector(velX, velY, velZ) * dt;
        vector newPos = pos + slideStep;
        float slideEdgeDist = BS_MapBounds.EdgeDistance(pos);
        float slideEdge = Math.Min(BS_MapBounds.GetEdge(), slideEdgeDist);
        vector slideSafe = BS_MapBounds.Clamp(newPos, slideEdge);
        if (slideSafe[0] != newPos[0]) velX = 0.0;
        if (slideSafe[2] != newPos[2]) velZ = 0.0;
        newPos = slideSafe;
        float floorGround = BS_CrashFloorY(newPos[0], newPos[2]);
        float floorY = floorGround + m_CrashBottom * 0.85;
        bool onGround = false;
        if (newPos[1] <= floorY)
        {
            newPos[1] = floorY;
            velY = 0.0;
            onGround = true;
        }
        m_CrashVelocity = Vector(velX, velY, velZ);

        float yawKeep = Math.Pow(Math.EULER, -dt / 0.4);
        m_CrashYawRate = m_CrashYawRate * yawKeep;
        m_CrashYaw = m_CrashYaw + m_CrashYawRate * dt;
        while (m_CrashYaw >= 360.0) m_CrashYaw = m_CrashYaw - 360.0;
        while (m_CrashYaw < 0.0) m_CrashYaw = m_CrashYaw + 360.0;
        BS_CrashAttitudeStep(dt, 0.0, 0.0, 3.5);

        if (CheckObjectCollision(pos, newPos))
        {
            CreateCollisionEffects(pos);
            BS_CrashBeginFinal(pos, floorGround);
            return;
        }

        SetPosition(newPos);
        SetOrientation(Vector(m_CrashYaw, m_CrashPitch, m_CrashRoll));

        if (m_CrashTotalTime - m_CrashLastTreeTime > 0.3)
        {
            m_CrashLastTreeTime = m_CrashTotalTime;
            DestroyNearbyTrees(newPos, 8.0);
        }
        float sinceDust = m_CrashTotalTime - m_CrashLastDustTime;
        if (onGround && newHoriz > 3.0 && sinceDust > 0.45)
        {
            m_CrashLastDustTime = m_CrashTotalTime;
            BS_RPCHelper.SendToNearby_Param1Vec(this, BS_RPCIds.BOUNCE_IMPACT, newPos, newPos, 500.0);
        }
        SendVelSyncIfDue(dt);

        bool slowEnough = (newHoriz < 3.0 && onGround);
        if (slowEnough || m_CrashSlideTime > CRASH_SLIDE_MAX_TIME)
            BS_CrashBeginFinal(newPos, floorGround);
    }

    void BS_CrashBeginFinal(vector pos, float groundHeight)
    {
        if (m_CrashImpactPending) return;
        m_CrashImpactPending = true;
        m_CrashImpactTimer = CRASH_IMPACT_HOLD;
        m_CrashImpactPos = pos;
        m_CrashImpactGround = groundHeight;
        m_CrashVelocity = vector.Zero;
        m_CrashYawRate = 0.0;
        SetPosition(pos);
        SetOrientation(Vector(m_CrashYaw, m_CrashPitch, m_CrashRoll));
        SendVelSyncIfDue(0.025, true);
    }

    void BS_BreakTail()
    {
        m_BS_TailBreakPending = false;
        string tailClass = BS_GetHeliConfigText("bsTailClass");
        if (tailClass == "") return;
        m_BS_TailBroken = true;
        SetAnimationPhase("bs_tail_hide", 1.0);

        vector heliMat[4];
        GetTransform(heliMat);
        vector originLocal = BS_ModelPoint("bs_tail_origin", TAIL_ORIGIN_DEFAULT);
        vector p0 = ModelToWorld(originLocal);
        vector arm = p0 - heliMat[3];

        float yawRateRad = m_CrashYawRate * Math.DEG2RAD;
        float flingX = yawRateRad * arm[2];
        float flingZ = -yawRateRad * arm[0];
        float flingLen = Math.Sqrt(flingX * flingX + flingZ * flingZ);
        if (flingLen > TAIL_FLING_MAX)
        {
            float flingScale = TAIL_FLING_MAX / flingLen;
            flingX = flingX * flingScale;
            flingZ = flingZ * flingScale;
        }
        vector v0 = m_CrashVelocity + Vector(flingX, -1.5, flingZ);

        float tumble = Math.RandomFloat(110.0, 190.0);
        if (Math.RandomFloat01() > 0.5) tumble = -tumble;
        float twist = Math.RandomFloat(30.0, 80.0);
        if (Math.RandomFloat01() > 0.5) twist = -twist;
        float tailYawRate = m_CrashYawRate * 0.8;
        vector w = Vector(0, tailYawRate, 0) + heliMat[0] * tumble + heliMat[2] * twist;

        float landT = BS_HeliTailMath.FindLanding(p0, v0);
        vector landPos = BS_HeliTailMath.PosAt(p0, v0, landT);
        float tailEdge = BS_MapBounds.GetEdge();
        vector landSafe = BS_MapBounds.Clamp(landPos, tailEdge);
        bool tailOutX = (landSafe[0] != landPos[0]);
        bool tailOutZ = (landSafe[2] != landPos[2]);
        if (tailOutX || tailOutZ)
        {
            float tailDecay = 1.0 - Math.Pow(Math.EULER, -BS_HeliTailMath.DRAG * landT);
            float tailK = tailDecay / BS_HeliTailMath.DRAG;
            if (tailK > 0.001)
            {
                float tailVX = (landSafe[0] - p0[0]) / tailK;
                float tailVY = v0[1];
                float tailVZ = (landSafe[2] - p0[2]) / tailK;
                v0 = Vector(tailVX, tailVY, tailVZ);
                landPos = BS_HeliTailMath.PosAt(p0, v0, landT);
            }
            landPos = BS_MapBounds.Clamp(landPos, tailEdge);
        }
        float landGround = GetGame().SurfaceY(landPos[0], landPos[2]);
        vector landProbe = Vector(landPos[0], landGround, landPos[2]);
        float landWater = GetGame().GetWaterDepth(landProbe);
        bool onWater = (landWater > 0.6);

        vector dirAtLand = BS_HeliTailMath.Rotate(heliMat[2], w, landT);
        float restYaw = Math.Atan2(dirAtLand[0], dirAtLand[2]) * Math.RAD2DEG;
        float restRoll = Math.RandomFloat(36.0, 44.0);
        if (twist < 0) restRoll = -restRoll;
        float restPitch = Math.RandomFloat(-6.0, 4.0);
        vector restOri = Vector(restYaw, restPitch, restRoll);
        vector restPos = Vector(landPos[0], landGround, landPos[2]);

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        ScriptRPC tailRpc = new ScriptRPC();
        tailRpc.Write(v0);
        tailRpc.Write(w);
        tailRpc.Write(landT);
        tailRpc.Write(restPos);
        tailRpc.Write(restOri);
        tailRpc.Write(onWater);
        foreach (Man tailPlayer : players)
        {
            if (!tailPlayer) continue;
            PlayerIdentity tailIdentity = tailPlayer.GetIdentity();
            if (!tailIdentity) continue;
            float tailPlayerDist = vector.Distance(tailPlayer.GetPosition(), p0);
            if (tailPlayerDist <= TAIL_RPC_DISTANCE)
                tailRpc.Send(this, BS_RPCIds.TAIL_BREAK, true, tailIdentity);
        }

        if (!onWater)
        {
            float landMsF = landT * 1000.0;
            int landMs = landMsF;
            BS_TailLandTask.Schedule(tailClass, restPos, restOri, landMs);
        }
        BS_Patrol_Log.GetInstance().Info("Crash: tail broke off, lands in " + landT.ToString() + " s");
    }

    #endif

    void BS_OnTailBreakClient(vector v0, vector w, float landT, vector restPos, vector restOri, bool onWater)
    {
        if (m_BS_TailBroken) return;
        m_BS_TailBroken = true;
        SetAnimationPhase("bs_tail_hide", 1.0);
        StopParticle(m_CrashFireTail);
        StopParticle(m_Ptc_TailFire);
        StopParticle(m_Ptc_RotorDmg);
        if (m_BS_HeliBlinkerLight) { m_BS_HeliBlinkerLight.FadeOut(); m_BS_HeliBlinkerLight = null; }

        vector cutLocal = BS_ModelPoint("bs_tail_cut", TAIL_CUT_DEFAULT);
        vector cutWorld = ModelToWorld(cutLocal);
        ParticleManager pm = ParticleManager.GetInstance();
        if (pm)
        {
            m_CrashFireTail = pm.PlayOnObject(ParticleList.CAMP_NORMAL_FIRE, this, cutLocal, "0 0 0");
            m_BS_StumpSmoke = pm.PlayOnObject(ParticleList.SMOKE_GENERIC_WRECK, this, cutLocal, "0 0 0");
            pm.PlayInWorld(ParticleList.IMPACT_METAL_RICOCHET, cutWorld);
        }
        EffectSound snapSound = SEffectManager.PlaySound("offroad_hit_heavy_SoundSet", cutWorld);
        if (snapSound) snapSound.SetSoundAutodestroy(true);

        string fallClass = BS_GetHeliConfigText("bsTailFallingClass");
        if (fallClass == "") return;
        vector heliMat[4];
        GetTransform(heliMat);
        vector originLocal = BS_ModelPoint("bs_tail_origin", TAIL_ORIGIN_DEFAULT);
        vector p0 = ModelToWorld(originLocal);
        Object fallObj = GetGame().CreateObjectEx(fallClass, p0, ECE_LOCAL);
        BS_HeliTailFalling falling = BS_HeliTailFalling.Cast(fallObj);
        if (falling)
            falling.BS_Start(p0, heliMat[0], heliMat[1], heliMat[2], v0, w, landT, restPos, restOri, onWater);
        else if (fallObj)
            GetGame().ObjectDelete(fallObj);
    }

    float BS_CrashFloorY(float x, float z)
    {
        float groundY = GetGame().SurfaceY(x, z);
        vector floorProbe = Vector(x, groundY, z);
        float waterDepth = GetGame().GetWaterDepth(floorProbe);
        m_CrashOverWater = (waterDepth > 0.3);
        if (m_CrashOverWater)
            return groundY + waterDepth;
        return groundY;
    }

    void BS_SetCrashMode(int mode)
    {
        m_CrashMode = mode;
        if (mode == 1)
        {
            m_CrashSpinTarget = 45.0;
            m_CrashSpinTau = 1.5;
            m_CrashDescent = 24.0;
            m_CrashDescentTau = 1.0;
            m_CrashHorizTau = 8.0;
            m_CrashPitchTarget = 15.0;
            m_CrashRollTarget = 7.0;
            m_TailRotorFailed = false;
            m_MainRotorFailed = true;
            BS_Patrol_Log.GetInstance().Info("Crash: Engine destroyed - powerless dive");
        }
        else if (mode == 2)
        {
            m_CrashSpinTarget = 310.0;
            m_CrashSpinTau = 0.8;
            m_CrashDescent = 32.0;
            m_CrashDescentTau = 0.8;
            m_CrashHorizTau = 4.0;
            m_CrashPitchTarget = 13.0;
            m_CrashRollTarget = 22.0;
            m_TailRotorFailed = true;
            m_MainRotorFailed = true;
            BS_Patrol_Log.GetInstance().Info("Crash: Tail + Engine destroyed - critical fall");
        }
        else
        {
            m_CrashSpinTarget = 250.0;
            m_CrashSpinTau = 0.9;
            m_CrashDescent = 25.0;
            m_CrashDescentTau = 1.0;
            m_CrashHorizTau = 5.0;
            m_CrashPitchTarget = 9.0;
            m_CrashRollTarget = 16.0;
            m_TailRotorFailed = true;
            m_MainRotorFailed = false;
            BS_Patrol_Log.GetInstance().Info("Crash: Tail destroyed - violent spin");
        }
    }

    void FinalCrash(vector crashPos, float groundHeight)
    {
        m_IsCrashed = true;
        m_IsCrashing = false;
        float wreckEdge = BS_MapBounds.GetEdge();
        crashPos = BS_MapBounds.Clamp(crashPos, wreckEdge);

        string wreckType = "BS_AH64D_Crashed";
        string heliWreck = BS_GetHeliConfigText("bsWreckClass");
        if (heliWreck != "")
            wreckType = heliWreck;
        string crashAmmo = "Explosion_NonLethal";
        float crashTrees = 18.0;
        ref BS_HeliConfig wreckCfg = GetConfig();
        if (wreckCfg && wreckCfg.WreckSettings)
        {
            string jsonWreck = wreckCfg.WreckSettings.WreckClassName;
            if (jsonWreck != "" && jsonWreck != "BS_AH64D_Crashed")
                wreckType = jsonWreck;
            string jsonAmmo = wreckCfg.WreckSettings.CrashExplosionAmmo;
            if (jsonAmmo != "" && GetGame().ConfigIsExisting("CfgAmmo " + jsonAmmo))
                crashAmmo = jsonAmmo;
            if (wreckCfg.WreckSettings.CrashTreeRadius > 0)
                crashTrees = wreckCfg.WreckSettings.CrashTreeRadius;
        }
        string noTailWreck = BS_GetHeliConfigText("bsWreckNoTailClass");
        if (m_BS_TailBroken && noTailWreck != "" && wreckType == heliWreck)
            wreckType = noTailWreck;
        
        float heightOffset = 1.25; 
        vector surfaceNormal = GetGame().GetSurfaceOrientation(crashPos[0], crashPos[2]);
        vector ori = GetOrientation();
        float finalRoll = 0.0;
        float finalPitch = 0.0;
        vector crashOri = Vector(m_CrashYaw, finalPitch, finalRoll);
        crashPos[1] = groundHeight + heightOffset;
        Object wreckObj = GetGame().CreateObject(wreckType, crashPos, false, true);
        
        if (wreckObj)
        {
            wreckObj.SetOrientation(crashOri);
            BS_AH64D_Crashed apacheWreck = BS_AH64D_Crashed.Cast(wreckObj);
            if (apacheWreck)
            {
                wreckObj.SetPosition(crashPos);
                wreckObj.SetOrientation(crashOri);
                BS_PlaceWreckOnTerrain(wreckObj, crashPos, m_CrashYaw, heightOffset);
                apacheWreck.m_PresetName = m_PresetName;
                apacheWreck.m_Config = m_Config; // propagar config del preset al wreck
                apacheWreck.CrashedEvent();
            }
        }
        
        #ifdef SERVER
        DamageSystem.ExplosionDamage(this, null, crashAmmo, crashPos, DamageType.EXPLOSION);
        DestroyNearbyTrees(crashPos, crashTrees);
        #endif

        CreateFinalExplosion(crashPos);
        DamageNearbyEntities(crashPos);
        OnCrashImpactFX(crashPos);
        CleanupAllParticlesAndEffects();
        GetGame().ObjectDelete(this);
        BS_Patrol_Log.GetInstance().Info("Crash finalized at: " + crashPos.ToString());
    }

    static const float WRECK_TILT_MAX = 20.0;
    static const float WRECK_SAMPLE_LEN = 5.5;
    static const float WRECK_SAMPLE_WID = 2.5;

    void BS_PlaceWreckOnTerrain(Object wreck, vector center, float yawDeg, float lift)
    {
        if (!wreck) return;
        if (wreck.MemoryPointExists("bs_ground_0"))
        {
            BS_PlaceWreckByContacts(wreck, center, yawDeg);
            return;
        }

        float yawRad = yawDeg * Math.DEG2RAD;
        vector fwd = Vector(Math.Sin(yawRad), 0, Math.Cos(yawRad));
        vector side = Vector(fwd[2], 0, -fwd[0]);

        vector pF = center + fwd * WRECK_SAMPLE_LEN;
        vector pB = center - fwd * WRECK_SAMPLE_LEN;
        vector pR = center + side * WRECK_SAMPLE_WID;
        vector pL = center - side * WRECK_SAMPLE_WID;
        pF[1] = GetGame().SurfaceY(pF[0], pF[2]);
        pB[1] = GetGame().SurfaceY(pB[0], pB[2]);
        pR[1] = GetGame().SurfaceY(pR[0], pR[2]);
        pL[1] = GetGame().SurfaceY(pL[0], pL[2]);

        vector axisF = pF - pB;
        vector axisR = pR - pL;
        float upX = axisF[1] * axisR[2] - axisF[2] * axisR[1];
        float upY = axisF[2] * axisR[0] - axisF[0] * axisR[2];
        float upZ = axisF[0] * axisR[1] - axisF[1] * axisR[0];
        vector up = Vector(upX, upY, upZ);
        up.Normalize();
        float maxTiltCos = Math.Cos(WRECK_TILT_MAX * Math.DEG2RAD);
        if (up[1] < maxTiltCos) return;

        vector dir = axisF;
        dir.Normalize();
        float planeY = (pF[1] + pB[1] + pR[1] + pL[1]) * 0.25;
        vector wreckPos = Vector(center[0], planeY, center[2]) + up * lift;

        vector wreckMat[4];
        Math3D.DirectionAndUpMatrix(dir, up, wreckMat);
        wreckMat[3] = wreckPos;
        wreck.SetTransform(wreckMat);
    }

    static const float WRECK_SEAT_TOLERANCE = 0.05;
    static const float WRECK_SEAT_MAX = 0.25;

    void BS_PlaceWreckByContacts(Object wreck, vector center, float yawDeg)
    {
        float lift = 1.25;
        if (wreck.MemoryPointExists("bs_ground_ref"))
        {
            vector groundRef = wreck.GetMemoryPointPos("bs_ground_ref");
            lift = -groundRef[1];
        }

        float sW = 0;
        float sX = 0;
        float sZ = 0;
        float sXX = 0;
        float sXZ = 0;
        float sZZ = 0;
        float sH = 0;
        float sXH = 0;
        float sZH = 0;
        int contactCount = 0;
        for (int i = 0; i < 16; i++)
        {
            string contactName = "bs_ground_" + i.ToString();
            if (!wreck.MemoryPointExists(contactName))
                break;
            vector contactLocal = wreck.GetMemoryPointPos(contactName);
            vector contactWorld = wreck.ModelToWorld(contactLocal);
            float contactGround = GetGame().SurfaceY(contactWorld[0], contactWorld[2]);
            float wgt = 1.0;
            if (i <= 5) wgt = 2.0;
            float relX = contactWorld[0] - center[0];
            float relZ = contactWorld[2] - center[2];
            sW += wgt;
            sX += wgt * relX;
            sZ += wgt * relZ;
            sXX += wgt * relX * relX;
            sXZ += wgt * relX * relZ;
            sZZ += wgt * relZ * relZ;
            sH += wgt * contactGround;
            sXH += wgt * relX * contactGround;
            sZH += wgt * relZ * contactGround;
            contactCount++;
        }
        if (contactCount < 3) return;

        float det = sW * (sXX * sZZ - sXZ * sXZ) - sX * (sX * sZZ - sXZ * sZ) + sZ * (sX * sXZ - sXX * sZ);
        if (Math.AbsFloat(det) < 0.000001) return;
        float detA = sH * (sXX * sZZ - sXZ * sXZ) - sX * (sXH * sZZ - sXZ * sZH) + sZ * (sXH * sXZ - sXX * sZH);
        float detB = sW * (sXH * sZZ - sZH * sXZ) - sH * (sX * sZZ - sXZ * sZ) + sZ * (sX * sZH - sXH * sZ);
        float detC = sW * (sXX * sZH - sXZ * sXH) - sX * (sX * sZH - sXH * sZ) + sH * (sX * sXZ - sXX * sZ);
        float planeA = detA / det;
        float slopeX = detB / det;
        float slopeZ = detC / det;

        float slopeLen = Math.Sqrt(slopeX * slopeX + slopeZ * slopeZ);
        float slopeMax = Math.Tan(WRECK_TILT_MAX * Math.DEG2RAD);
        if (slopeLen > slopeMax)
        {
            float slopeScale = slopeMax / slopeLen;
            slopeX = slopeX * slopeScale;
            slopeZ = slopeZ * slopeScale;
        }
        vector up = Vector(-slopeX, 1.0, -slopeZ);
        up.Normalize();

        float yawRad = yawDeg * Math.DEG2RAD;
        vector fwd = Vector(Math.Sin(yawRad), 0, Math.Cos(yawRad));
        float fwdUp = vector.Dot(fwd, up);
        vector dir = fwd - up * fwdUp;
        dir.Normalize();

        vector wreckPos = Vector(center[0], planeA, center[2]) + up * lift;
        vector wreckMat[4];
        Math3D.DirectionAndUpMatrix(dir, up, wreckMat);
        wreckMat[3] = wreckPos;
        wreck.SetTransform(wreckMat);

        float maxFloat = -1000.0;
        for (int j = 0; j <= 5; j++)
        {
            string seatName = "bs_ground_" + j.ToString();
            if (!wreck.MemoryPointExists(seatName))
                break;
            vector seatLocal = wreck.GetMemoryPointPos(seatName);
            vector seatWorld = wreck.ModelToWorld(seatLocal);
            float seatGround = GetGame().SurfaceY(seatWorld[0], seatWorld[2]);
            float floatH = seatWorld[1] - seatGround;
            if (floatH > maxFloat) maxFloat = floatH;
        }
        if (maxFloat > WRECK_SEAT_TOLERANCE)
        {
            float drop = Math.Min(maxFloat - WRECK_SEAT_TOLERANCE, WRECK_SEAT_MAX);
            wreckMat[3] = wreckPos - Vector(0, drop, 0);
            wreck.SetTransform(wreckMat);
        }
    }

    bool CheckObjectCollision(vector fromPos, vector toPos)
    {
        // Rayo central
        vector hitPos, hitNormal;
        int hitComponent;
        set<Object> hitObjects = new set<Object>();
        
        if (DayZPhysics.RaycastRV(fromPos, toPos, hitPos, hitNormal, hitComponent, hitObjects, null, this, false, false, ObjIntersectGeom))
        {
            if (hitObjects.Count() > 0)
            {
                Object hitObj = hitObjects[0];
                if (hitObj.IsTree() || hitObj.IsBuilding() || hitObj.IsRock()) return true;
            }
        }
        
        // Rayos laterales para cubrir el ancho del heli (~4m cada lado)
        vector moveDir = toPos - fromPos;
        moveDir[1] = 0;
        float moveDirLen = moveDir.Length();
        if (moveDirLen < 0.001) return false;
        moveDir = moveDir * (1.0 / moveDirLen);
        
        // Vector perpendicular en el plano XZ
        vector perpDir = Vector(-moveDir[2], 0, moveDir[0]);
        float heliHalfWidth  = 4.0;
        float heliHalfLength = 5.0;
        
        // 4 esquinas del bbox simplificado
        vector offsets[4];
        offsets[0] = perpDir *  heliHalfWidth + moveDir * (-heliHalfLength);
        offsets[1] = perpDir * -heliHalfWidth + moveDir * (-heliHalfLength);
        offsets[2] = perpDir *  heliHalfWidth + moveDir *   heliHalfLength;
        offsets[3] = perpDir * -heliHalfWidth + moveDir *   heliHalfLength;
        
        for (int i = 0; i < 4; i++)
        {
            vector rayFrom = fromPos + offsets[i];
            vector rayTo   = toPos   + offsets[i];
            
            set<Object> sideHits = new set<Object>();
            vector sHitPos, sHitNorm;
            int sHitComp;
            
            if (DayZPhysics.RaycastRV(rayFrom, rayTo, sHitPos, sHitNorm, sHitComp, sideHits, null, this, false, false, ObjIntersectGeom))
            {
                if (sideHits.Count() > 0)
                {
                    Object sHitObj = sideHits[0];
                    if (sHitObj.IsTree() || sHitObj.IsBuilding() || sHitObj.IsRock()) return true;
                }
            }
        }
        
        return false;
    }

    void UpdateCrashEffects(float dt, float heightAGL)
    {
        if (m_CrashAlarmSound) m_CrashAlarmSound.SetPosition(GetPosition());
        if (m_CrashWindSound) m_CrashWindSound.SetPosition(GetPosition());
    }

    void SpawnSmokeTrail(vector pos)
    {
        BS_RPCHelper.SendToNearby_Param1Vec(this, BS_RPCIds.SMOKE_TRAIL, pos, pos, 800.0);
    }

    void CreateBounceEffects(vector pos)
    {
        BS_RPCHelper.SendToNearby_Param1Vec(this, BS_RPCIds.BOUNCE_IMPACT, pos, pos, 500.0);
    }

    void CreateSkidEffects(vector pos) { }

    void CreateCollisionEffects(vector pos) { }

    void CreateFinalExplosion(vector pos)
    {
        BS_RPCHelper.SendToNearby_Param1Vec(this, BS_RPCIds.FINAL_EXPLOSION, pos, pos, 1000.0);
    }

    void DamageNearbyEntities(vector crashPos)
    {
        #ifdef SERVER
        BS_DifficultyConfig diff = GetDiff();
        float damageRadius = diff.CrashDamageRadius;
        float maxDamage = diff.CrashDamageMax;
        
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        
        foreach (Man player : players)
        {
            if (!player) continue;
            float dist = vector.Distance(player.GetPosition(), crashPos);
            if (dist <= damageRadius)
            {
                float damageFactor = 1.0 - (dist / damageRadius);
                float damage = maxDamage * damageFactor * damageFactor;
                player.DecreaseHealth("", "", damage);
            }
        }
        #endif
    }

    void DestroyNearbyTrees(vector centerPos, float radius)
    {
        #ifdef SERVER
        array<Object> nearbyObjects = new array<Object>;
        array<CargoBase> proxyCargos = new array<CargoBase>;
        GetGame().GetObjectsAtPosition(centerPos, radius, nearbyObjects, proxyCargos);

        foreach (Object obj : nearbyObjects)
        {
            WoodBase tree = WoodBase.Cast(obj);
            if (tree && tree.IsTree() && !tree.IsDamageDestroyed())
            {
                tree.SetAllowDamage(true);
                tree.DecreaseHealth("", "", tree.GetMaxHealth(), false);
                if (tree.IsDamageDestroyed())
                    tree.OnTreeCutDown(this);
            }
        }
        
        BS_Patrol_Log.GetInstance().Info("[BS_Apache] Trees destroyed around crash site. Radius: " + radius + "m");
        #endif
    }

    void SendCrashStartToClients()
    {
        #ifdef SERVER
        int failureType = 0;
        if (m_TailRotorFailed) failureType = 1;
        BS_RPCHelper.SendToNearby_Param3VIF(this, BS_RPCIds.CRASH_START, GetPosition(), failureType, m_SpinDirection, GetPosition(), 1500.0);
        #endif
    }

    void CreateCrashEffects()
    {
        if (!GetGame().IsClient() && GetGame().IsMultiplayer()) return;
        
        StopParticle(m_Ptc_BladeFire);
        StopParticle(m_Ptc_TailFire);
        StopParticle(m_Ptc_BladeDmg);

        // Apagar searchlight al crashear
        if (m_BS_HeliSearchLight) { m_BS_HeliSearchLight.FadeOut(); m_BS_HeliSearchLight = null; }

        m_CrashSmokeHeavy = ParticleManager.GetInstance().PlayOnObject(ParticleList.SMOKE_GENERIC_WRECK, this, GetMemoryPointPos("engine1_axis"), "0 0 0");
        m_CrashFireEngine = ParticleManager.GetInstance().PlayOnObject(ParticleList.BONFIRE_FIRE, this, GetMemoryPointPos("engine1_axis"), "0 0 0");
        m_CrashFireTail   = ParticleManager.GetInstance().PlayOnObject(ParticleList.CAMP_NORMAL_FIRE, this, GetMemoryPointPos("engine2_axis"), "0 0 0");
    }

    float ComputeTerrainClearance(vector pos)
    {

        vector velocityDir = GetDirection();
        vector futurePos = pos + (velocityDir * 25.0); // Miramos 25 metros hacia adelante
        
        vector rayStart = Vector(futurePos[0], futurePos[1] + 10.0, futurePos[2]);
        vector rayEnd = Vector(futurePos[0], futurePos[1] - 45.0, futurePos[2]);

        vector hitPos, hitNormal;
        int hitComp;
        set<Object> hitObjects = new set<Object>();

        if (DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComp, hitObjects, null, this, false, false, ObjIntersectGeom))
        {
            float clearance = pos[1] - hitPos[1];
            float safeMargin = 40.0;
            
            if (clearance < safeMargin)
            {
                float neededBoost = (safeMargin - clearance) * 0.6;
                return Math.Clamp(neededBoost, 0.0, 20.0);
            }
        }
        
        return 0.0;
    }

    float GetNormalizedPhase(float currentAngle, float minLimit, float maxLimit)
    {
        float clampedAngle = Math.Clamp(currentAngle, minLimit, maxLimit);
        float totalRange = Math.Max(0.001, maxLimit - minLimit);
        return (clampedAngle - minLimit) / totalRange;
    }

    protected vector BS_ModelPoint(string pointName, vector fallback)
    {
        if (MemoryPointExists(pointName))
            return GetMemoryPointPos(pointName);
        return fallback;
    }

    protected void BS_EnsureModelPoints()
    {
        if (m_BS_PointsReady) return;
        m_BS_PointsReady = true;

        m_BS_PtMuzzle = BS_ModelPoint("bs_gun_muzzle", POS_GUN_MUZZLE);
        m_BS_HasGunPivot = MemoryPointExists("bs_gun_pivot");
        if (m_BS_HasGunPivot)
            m_BS_PtGunPivot = GetMemoryPointPos("bs_gun_pivot");
        m_BS_PtMainRotor = BS_ModelPoint("bs_main_rotor", POS_MAIN_ROTOR);
        m_BS_PtTailRotor = BS_ModelPoint("bs_tail_rotor", POS_TAIL_ROTOR);
        m_BS_PtTailLight = BS_ModelPoint("bs_tail_light", POS_TAIL_LIGHT);
        m_BS_PtFlareL = BS_ModelPoint("bs_flare_l", BS_FlareManager.DISPENSER_LEFT);
        m_BS_PtFlareR = BS_ModelPoint("bs_flare_r", BS_FlareManager.DISPENSER_RIGHT);

        bool hasRailL = MemoryPointExists("bs_missile_l");
        bool hasRailR = MemoryPointExists("bs_missile_r");
        m_BS_HasMissileRails = hasRailL && hasRailR;
        m_BS_PtMissileL = POS_MISSILE_LEFT;
        m_BS_PtMissileR = POS_MISSILE_RIGHT;
        if (m_BS_HasMissileRails)
        {
            m_BS_PtMissileL = GetMemoryPointPos("bs_missile_l");
            m_BS_PtMissileR = GetMemoryPointPos("bs_missile_r");
        }
    }

    vector BS_GetMuzzleLocal()
    {
        BS_EnsureModelPoints();
        if (!m_BS_HasGunPivot) return m_BS_PtMuzzle;

        float yawPhase = GetAnimationPhase(ANIM_GUN_YAW);
        float pitchPhase = GetAnimationPhase(ANIM_GUN_PITCH);
        float yawRad = (-170.0 + yawPhase * 340.0) * Math.DEG2RAD;
        float pitchRad = (45.0 - pitchPhase * 90.0) * Math.DEG2RAD;

        vector arm = m_BS_PtMuzzle - m_BS_PtGunPivot;
        float cosP = Math.Cos(pitchRad);
        float sinP = Math.Sin(pitchRad);
        float cosY = Math.Cos(yawRad);
        float sinY = Math.Sin(yawRad);
        float armY = arm[1] * cosP + arm[2] * sinP;
        float armZ = arm[2] * cosP - arm[1] * sinP;
        float armX = arm[0] * cosY + armZ * sinY;
        float armZ2 = armZ * cosY - arm[0] * sinY;
        return m_BS_PtGunPivot + Vector(armX, armY, armZ2);
    }

    vector BS_GetMuzzleWorld()
    {
        vector muzzleLocal = BS_GetMuzzleLocal();
        return ModelToWorld(muzzleLocal);
    }

    vector BS_GetMainRotorLocal()
    {
        BS_EnsureModelPoints();
        return m_BS_PtMainRotor;
    }

    vector BS_GetTailRotorLocal()
    {
        BS_EnsureModelPoints();
        return m_BS_PtTailRotor;
    }

    vector BS_GetTailLightLocal()
    {
        BS_EnsureModelPoints();
        return m_BS_PtTailLight;
    }

    vector BS_GetFlareDispenserLocal(bool left)
    {
        BS_EnsureModelPoints();
        if (left) return m_BS_PtFlareL;
        return m_BS_PtFlareR;
    }

    vector BS_GetMissileLaunchLocal(bool left)
    {
        BS_EnsureModelPoints();
        if (left) return m_BS_PtMissileL;
        return m_BS_PtMissileR;
    }

    string BS_GetHeliConfigText(string entry)
    {
        string cfgPath = "CfgVehicles " + GetType() + " " + entry;
        string cfgValue = "";
        if (GetGame().ConfigIsExisting(cfgPath))
            GetGame().ConfigGetText(cfgPath, cfgValue);
        return cfgValue;
    }

    void SetupVisualEffects()
    {
        if (!m_BS_LightEnabled || m_BS_HeliSearchLight) return;
        if (GetGame().IsClient() || !GetGame().IsMultiplayer())
        {
            m_BS_HeliSearchLight = BS_HeliSearchLight.Cast(ScriptedLightBase.CreateLight(BS_HeliSearchLight, "0 0 0", 0));
            if (m_BS_HeliSearchLight)
            {
                m_BS_HeliSearchLight.BS_Configure(m_BS_LightBrightness, m_BS_LightRadius, m_BS_LightAngle);
                if (MemoryPointExists("spot_light_dir"))
                    m_BS_HeliSearchLight.AttachOnMemoryPoint(this, "spot_light", "spot_light_dir");
                else
                    m_BS_HeliSearchLight.AttachOnMemoryPoint(this, "spot_light");
            }
        }
    }

    void PlayFlareFX(vector posLeft, vector posRight)
    {
        if (GetGame().IsClient() || !GetGame().IsMultiplayer())
        {
            ParticleManager.GetInstance().PlayInWorld(ParticleList.ROADFLARE_BURNING_MAIN, posLeft);
            ParticleManager.GetInstance().PlayInWorld(ParticleList.ROADFLARE_BURNING_MAIN, posRight);
        }
    }

    void OnCrashImpactFX(vector crashPos) 
    {        
        if (GetGame().IsServer() && GetGame().IsMultiplayer())
        {
            BS_RPCHelper.SendToNearby_Param1Vec(this, BS_RPCIds.CRASH_SOUND, crashPos, crashPos, 1500.0);
            return;
        }

        if (GetGame().IsClient() || !GetGame().IsMultiplayer()) 
        {
            string soundSet = "GasStation_Explosion_SoundSet";
            if (GetType().IndexOf("BS_AH64D_Apache") == 0) soundSet = "HelicopterCrash_SoundSets_apache";
            string heliCrashSound = BS_GetHeliConfigText("bsCrashSoundSet");
            if (heliCrashSound != "") soundSet = heliCrashSound;
            
            EffectSound crashBoom = SEffectManager.PlaySound(soundSet, crashPos);
            if (crashBoom) crashBoom.SetSoundAutodestroy(true);
        }
    }

    void OnGunFiredFX(vector muzzleWS, vector endPos) 
    {        
        if (GetGame().IsClient() || !GetGame().IsMultiplayer()) 
        {
            EffectSound bang = SEffectManager.PlaySound("BS_Shot_SoundSet", muzzleWS);
            if (bang) bang.SetSoundAutodestroy(true);
            
            PlayerBase shotListener = PlayerBase.Cast(GetGame().GetPlayer());
            if (shotListener && vector.DistanceSq(endPos, shotListener.GetPosition()) < 2500)
            {
                EffectSound flyby = SEffectManager.PlaySound("bulletFlyBy_SoundSet", endPos);
                if (flyby) flyby.SetSoundAutodestroy(true);
            }

            m_Ptc_MuzzleFlash = ParticleManager.GetInstance().PlayInWorld(ParticleList.GUN_FNX, muzzleWS);
            m_Ptc_MuzzleSmoke = ParticleManager.GetInstance().PlayInWorld(ParticleList.SMOKING_BARREL_SMALL, muzzleWS);

            vector dir = endPos - muzzleWS;
            Object tracerObj = GetGame().CreateObjectEx("Patrol_Tracer", muzzleWS, ECE_LOCAL);
            if (tracerObj) 
            {
                Patrol_Tracer tracer = Patrol_Tracer.Cast(tracerObj);
                if (tracer) tracer.Initialize(dir, 600.0); 
            }
        }
    }

    void CleanupParticles() 
    {
        if (m_Ptc_RotorDmg)     { m_Ptc_RotorDmg.Stop();     m_Ptc_RotorDmg = null; }
        if (m_Ptc_BladeDmg)     { m_Ptc_BladeDmg.Stop();     m_Ptc_BladeDmg = null; }
        if (m_Ptc_BladeFire)    { m_Ptc_BladeFire.Stop();    m_Ptc_BladeFire = null; }
        if (m_Ptc_MuzzleFlash)  { m_Ptc_MuzzleFlash.Stop();  m_Ptc_MuzzleFlash = null; }
        if (m_Ptc_MuzzleSmoke)  { m_Ptc_MuzzleSmoke.Stop();  m_Ptc_MuzzleSmoke = null; }
        if (m_Ptc_TailFire)     { m_Ptc_TailFire.Stop();     m_Ptc_TailFire = null; }
        if (m_Ptc_Exhaust1)     { m_Ptc_Exhaust1.Stop();     m_Ptc_Exhaust1 = null; }
        if (m_Ptc_Exhaust2)     { m_Ptc_Exhaust2.Stop();     m_Ptc_Exhaust2 = null; }
    }

    override void EEDelete(EntityAI parent) 
    {
        CleanupAllParticlesAndEffects();
        
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(OnUpdate);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ClientUpdateLoop);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(RefreshDamageParticles);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(RefreshFlyingSound);
        
        #ifdef SERVER
        if (GetGame().IsServer())
        {
            #ifdef EXPANSIONMODNAVIGATION
            RemoveExpansionMarker();
            #endif
            #ifdef LBmaster_Groups
            RemoveLBMarker();
            #endif
            if (!m_IsCrashed && m_PresetName != "")
            {
                vector lostPos = GetPosition();
                if (m_BS_LeftArea)
                    BS_Patrol_Log.GetInstance().Info("[" + m_PresetName + "] Heli left the area, event will respawn it.");
                else
                    BS_Patrol_Log.GetInstance().Warning("[BS_Apache] Heli deleted without crashing at " + lostPos.ToString() + ", event will respawn it.");
                BS_PatrolEventState.SetWreckDestroyed(m_PresetName);
            }
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(BS_AnnounceDestroyedBy);
        }
        #endif

        if (GetGame().IsClient() || !GetGame().IsMultiplayer()) 
        {
            if (m_BS_HeliBlinkerLight) { m_BS_HeliBlinkerLight.FadeOut(); m_BS_HeliBlinkerLight = null; }
            if (m_BS_HeliSearchLight) { m_BS_HeliSearchLight.FadeOut(); m_BS_HeliSearchLight = null; }
        }

        super.EEDelete(parent); 
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version) 
    {
        bool loaded = super.OnStoreLoad(ctx, version);
        
        if (loaded && GetGame().IsServer())
            GetGame().ObjectDelete(this);

        return loaded;
    }

    override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone) 
    {
        super.EEHealthLevelChanged(oldLevel, newLevel, zone);
        if (m_IsCrashing) return;
        
        if (m_CustomMaxHealth > 0) return;
        
        if (GetGame().IsClient() || !GetGame().IsMultiplayer()) 
        {
            if (newLevel > oldLevel) 
            {    
                ApplyDamageVisuals(newLevel);
            }
        }
        
        #ifdef SERVER
        if (GetGame().IsServer()) 
        {
            float health = GetHealth("", "Health");
            if (health <= 0 && !m_IsCrashing) 
            {
                InitiateCrashSequence();
            }
            else if (newLevel > oldLevel)
            {
                // Guardar y reenviar el nuevo nivel a clientes
                m_CurrentVisualDamageLevel = newLevel;
                SendDamageLevelToClients(newLevel);
            }
        }
        #endif
    }

    void RefreshFlyingSound()
    {
        if (m_IsCrashed || m_IsCrashing)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(RefreshFlyingSound);
            return;
        }
        if (m_FlyingSound && m_FlyingSound.IsSoundPlaying())
            return;
        PlayerBase listener = PlayerBase.Cast(GetGame().GetPlayer());
        if (listener && vector.DistanceSq(GetPosition(), listener.GetPosition()) > 2250000)
            return;
        if (m_FlyingSound)
            StopSoundSet(m_FlyingSound);
        PlaySoundSetLoop(m_FlyingSound, "BSPatrol_FlyingSoundSet", 0.5, 0.5);
    }

    protected int m_LastVisualDamageLevel = -1;

    void RefreshDamageParticles()
    {
        if (m_IsCrashed || m_IsCrashing)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(RefreshDamageParticles);
            return;
        }
        if (m_LastVisualDamageLevel > GameConstants.STATE_PRISTINE)
            ApplyDamageVisuals(m_LastVisualDamageLevel);
    }

    void ApplyDamageVisuals(int level)
    {
        if (m_BS_TailBroken) return;
        bool levelChanged = (level != m_LastVisualDamageLevel);
        m_LastVisualDamageLevel = level;
        
        if (levelChanged)
        {
            StopParticle(m_Ptc_RotorDmg); 
            StopParticle(m_Ptc_BladeDmg); 
            StopParticle(m_Ptc_BladeFire);
            StopParticle(m_Ptc_TailFire);
            if (m_BS_HeliBlinkerLight) { m_BS_HeliBlinkerLight.FadeOut(); m_BS_HeliBlinkerLight = null; }
        }

        vector mainRotorLocal = BS_GetMainRotorLocal();
        vector tailRotorLocal = BS_GetTailRotorLocal();
        vector tailLightLocal = BS_GetTailLightLocal();

        if (level >= GameConstants.STATE_RUINED)
        {
            if (!m_Ptc_BladeFire)
                m_Ptc_BladeFire = ParticleManager.GetInstance().PlayOnObject(ParticleList.BONFIRE_FIRE, this, mainRotorLocal, "0 0 0");
            if (!m_Ptc_TailFire)
                m_Ptc_TailFire = ParticleManager.GetInstance().PlayOnObject(ParticleList.CAMP_NORMAL_FIRE, this, tailRotorLocal, "0 0 0");
        }
        else if (level >= GameConstants.STATE_BADLY_DAMAGED)
        {
            if (!m_Ptc_BladeDmg)
                m_Ptc_BladeDmg = ParticleManager.GetInstance().PlayOnObject(ParticleList.BS_Patrol_Smoke, this, mainRotorLocal, "0 0 0");
        }
        else if (level >= GameConstants.STATE_DAMAGED)
        {
            if (!m_Ptc_RotorDmg)
                m_Ptc_RotorDmg = ParticleManager.GetInstance().PlayOnObject(ParticleList.SMOKING_BARREL_SMALL, this, tailRotorLocal, "0 0 0");

            if (!m_BS_HeliBlinkerLight)
            {
                m_BS_HeliBlinkerLight = BS_HeliBlinkerLight.Cast(ScriptedLightBase.CreateLight(BS_HeliBlinkerLight, "0 0 0", 0));
                if (m_BS_HeliBlinkerLight)
                    m_BS_HeliBlinkerLight.AttachOnObject(this, tailLightLocal);
            }
        }
    }

    void SendDamageLevelToClients(int level)
    {
        #ifdef SERVER
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        
        Param1<int> params = new Param1<int>(level);
        
        foreach (Man player : players)
        {
            if (!player) continue;
            float dist = vector.Distance(player.GetPosition(), GetPosition());
            if (dist <= DAMAGE_SYNC_DISTANCE)
            {
                PlayerIdentity identity = player.GetIdentity();
                if (identity)
                    GetGame().RPCSingleParam(this, BS_RPCIds.DAMAGE_LEVEL_SYNC, params, true, identity);
            }
        }
        #endif
    }

    void InitiateCrashSequence()
    {
        if (m_IsCrashing) return;
        
        m_IsCrashing = true;
        m_CrashStartTime = GetGame().GetTime();
        m_CrashPhase = 0;
        m_CrashPhaseTimer = 0.0;
        m_CrashTotalTime = 0.0;
        
        vector currentOri = GetOrientation();
        m_CrashYaw = currentOri[0];
        m_CrashPitch = currentOri[1];
        m_CrashRoll = currentOri[2];
        
        #ifdef SERVER
        if (m_PatrolController)
        {
            m_InitialForwardSpeed = m_PatrolController.m_CurrentSpeed;
            float yawRad = m_PatrolController.m_CurrentYaw * Math.DEG2RAD;
            m_InitialDirection = Vector(Math.Sin(yawRad), 0, Math.Cos(yawRad));
            
            m_CrashVelocity = m_InitialDirection * m_InitialForwardSpeed;
            m_CrashVelocity[1] = m_PatrolController.m_VerticalVelocity;
            m_CrashYawRate = m_PatrolController.m_YawRate;
        }
        else
        {
            m_InitialForwardSpeed = 15.0;
            m_InitialDirection = GetDirection();
            m_CrashVelocity = m_InitialDirection * m_InitialForwardSpeed;
            m_CrashVelocity[1] = 0.0;
        }
        #endif
        
        if (Math.RandomFloat01() > 0.5) m_SpinDirection = 1.0; else m_SpinDirection = -1.0;
        m_TailRotorFailure = Math.RandomFloat(0.85, 1.0);

        float modeRoll = Math.RandomFloat01();
        int crashMode = 0;
        if (modeRoll >= 0.45) crashMode = 1;
        if (modeRoll >= 0.80) crashMode = 2;
        string crashTailClass = BS_GetHeliConfigText("bsTailClass");
        bool hasBreakableTail = (crashTailClass != "");
        if (hasBreakableTail)
        {
            crashMode = 0;
            if (modeRoll >= 0.70) crashMode = 2;
        }
        BS_SetCrashMode(crashMode);
        m_BS_TailBreakPending = hasBreakableTail;
        m_CrashSliding = false;
        m_CrashSlideTime = 0.0;
        m_CrashImpactPending = false;
        m_CrashLastTickMS = -1;
        m_CrashPitchRate = 0.0;
        m_CrashRollRate = 0.0;
        m_CrashWobblePhase = Math.RandomFloat(0.0, 6.2832);

        m_CrashBottom = 2.0;
        vector crashBox[2];
        if (GetCollisionBox(crashBox))
        {
            float boxBottom = Math.Min(crashBox[0][1], crashBox[1][1]);
            m_CrashBottom = Math.Clamp(-boxBottom, 0.5, 4.0);
        }
        
        #ifdef SERVER
        if (m_PatrolController)
        {
            delete m_PatrolController;
            m_PatrolController = null;
        }
        #endif
        
        if (m_FlyingSound)
        {
            StopSoundSet(m_FlyingSound);
            m_FlyingSound = null;
        }
        
        CreateCrashEffects();
        
        #ifdef SERVER
        SendCrashStartToClients();
        #endif
        
        BS_Patrol_Log.GetInstance().Info("Crash started - Initial speed: " + m_InitialForwardSpeed + " m/s");
        ref BS_HeliConfig config = GetConfig();
        if (config && config.Notifications.EnableCrashNotification)
        {
             BS_NotificationHelper.Broadcast(config.Notifications.NotificationTitleKey, config.Notifications.CrashNotificationKey, config.Notifications.NotificationIcon);
        }
        #ifdef SERVER
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BS_AnnounceDestroyedBy, 0, false);
        #endif
    }

    void CleanupAllParticlesAndEffects()
    {
        StopParticle(m_Ptc_RotorDmg);
        StopParticle(m_Ptc_BladeDmg);
        StopParticle(m_Ptc_BladeFire);
        StopParticle(m_Ptc_MuzzleFlash);
        StopParticle(m_Ptc_MuzzleSmoke);
        StopParticle(m_Ptc_TailFire);
        StopParticle(m_Ptc_Exhaust1);
        StopParticle(m_Ptc_Exhaust2);

        StopParticle(m_CrashSmokeHeavy);
        StopParticle(m_CrashFireEngine);
        StopParticle(m_CrashFireTail);
        StopParticle(m_BS_StumpSmoke);
        StopParticle(m_CrashDebris);

        if (m_CrashAlarmSound) { m_CrashAlarmSound.SoundStop(); m_CrashAlarmSound = null; }
        if (m_CrashWindSound) { m_CrashWindSound.SoundStop(); m_CrashWindSound = null; }
        
        StopFlyingSoundClient();

        if (m_BS_HeliBlinkerLight) { m_BS_HeliBlinkerLight.FadeOut(); m_BS_HeliBlinkerLight = null; }
        if (m_BS_HeliSearchLight) { m_BS_HeliSearchLight.FadeOut(); m_BS_HeliSearchLight = null; }
    }

    void StopParticle(inout Particle p)
    {
        if (p)
        {
            p.Stop();
            p = null;
        }
    }

    // Expansion BaseBuilding aplica sus ajustes de raid (ProjectileDamageMultiplier, lista de explosivos) a todo
    // BuildingBase y con 0 cancelaba las balas contra el heli. El heli maneja su propio daño en EEHitBy.
    override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        return true;
    }

override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

        BS_DifficultyConfig diff = GetDiff();
        float now = GetGame().GetTime();

        float rawDamage = 0;
        if (damageResult) rawDamage = damageResult.GetDamage("", "Health");
        BS_HeliConfig hitCfg = GetConfig();
        float armorScale = 1.0;
        if (hitCfg) armorScale = BS_HeliArmor.Scale(hitCfg.HelicopterSettings, damageType, ammo);
        float damage = rawDamage * armorScale;
        if ((now - m_LastHitTime) > 10000)
            m_DamageAccumulator = 0;

        m_LastHitTime = now;
        m_DamageAccumulator += damage;

        SurvivorBase sb = ResolveAttacker(source);
        #ifdef SERVER
        if (sb)
        {
            PlayerIdentity hitIdentity = sb.GetIdentity();
            if (hitIdentity)
            {
                m_BS_LastAttackerName = hitIdentity.GetName();
                m_BS_LastAttackerMS = now;
            }
        }

        if (m_CustomMaxHealth <= 0 && !m_IsCrashing && armorScale != 1.0)
            AddHealth("", "Health", rawDamage - damage);

        if (m_CustomMaxHealth > 0 && !m_IsCrashing)
        {
            m_CustomCurrentHealth = m_CustomCurrentHealth - damage;
            if (m_CustomCurrentHealth < 0) m_CustomCurrentHealth = 0;
            
            float engineMax = GetMaxHealth("", "Health");
            SetHealth("", "Health", engineMax);
            
            float ratio = m_CustomCurrentHealth / m_CustomMaxHealth;
            int visualLevel = GameConstants.STATE_PRISTINE;
            if (ratio <= 0.0)
                visualLevel = GameConstants.STATE_RUINED;
            else if (ratio <= 0.3)
                visualLevel = GameConstants.STATE_RUINED;
            else if (ratio <= 0.5)
                visualLevel = GameConstants.STATE_BADLY_DAMAGED;
            else if (ratio <= 0.7)
                visualLevel = GameConstants.STATE_DAMAGED;

            if (visualLevel != m_CurrentVisualDamageLevel)
                SendDamageLevelToClients(visualLevel);
            m_CurrentVisualDamageLevel = visualLevel;
            
            if (m_CustomCurrentHealth <= 0 && !m_IsCrashing)
            {
                InitiateCrashSequence();
            }
        }
        #endif

        if (Math.RandomFloat01() < diff.FlareDeployChance)
        {
            #ifdef SERVER
            DeployFlares();
            #endif
        }
        
        if (sb && !BS_HeliTargets.Ignore(hitCfg, sb))
        {
            bool shouldSwitchTarget = false;

            if (!m_PrimaryTarget) 
            {
                shouldSwitchTarget = true;
            }
            else if (!m_PrimaryTarget.IsAlive() || m_PrimaryTarget.IsUnconscious())
            {
                shouldSwitchTarget = true;
            }
            else if ((now - m_TargetFocusTime) > 4000) 
            {
                shouldSwitchTarget = true;
            }
            
            if (shouldSwitchTarget)
            {
                m_PrimaryTarget = sb; 
                m_TimeSinceTargetAcquired = now;
                m_TargetFocusTime = now; 
                
                #ifdef SERVER
                EngagePatrolTarget(sb);
                #endif
            }
            
            // Registrar al atacante SIEMPRE, independientemente de si se cambio de target
            if (m_ActiveHostiles.Find(sb) == -1) m_ActiveHostiles.Insert(sb);
        }
        m_LastHitTimeMS = now;
    }
};

class Patrol_Tracer : House
{
    protected vector m_Direction;
    protected float m_FlightSpeed;
    protected float m_Lifetime;
    protected Particle m_ParticleTrail;

    void Patrol_Tracer() {}

    void ~Patrol_Tracer()
    {
        if (m_ParticleTrail) { m_ParticleTrail.Stop(); m_ParticleTrail = null; }
    }

    void Initialize(vector dir, float speed)
    {
        m_Direction = dir;
        m_Direction.Normalize();
        m_FlightSpeed = speed;

        SetOrientation(m_Direction.VectorToAngles());

        if (!m_ParticleTrail)
             m_ParticleTrail = ParticleManager.GetInstance().PlayOnObject(ParticleList.BS_TRACER_GREEN, this, "0 0 0", "0 0 0", true);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnUpdateLoop, 1, false); 
    }

    void OnUpdateLoop()
    {
        float dt = 0.016; 
        
        m_Lifetime = m_Lifetime + dt;
        if (m_Lifetime > 5.0)
        {
            GetGame().ObjectDelete(this);
            return;
        }

        vector newPos = GetPosition() + (m_Direction * m_FlightSpeed * dt);
        SetPosition(newPos);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnUpdateLoop, 16, false); 
    }
};

class BS_RocketLight: PointLightBase
{
    void BS_RocketLight()
    {
        SetFlareVisible(true);
        SetVisibleDuringDaylight(true);
        SetDiffuseColor(1.3, 0.35, 0.15);
        SetAmbientColor(1.3, 0.35, 0.15);
        SetBrightnessTo(100.0);
        SetRadiusTo(30.0);
        SetCastShadow(true);
        EnableHeatHaze(true);
        SetFlickerAmplitude(0.2);
        SetFlickerSpeed(1);
        SetHeatHazePower(0.5);
        SetHeatHazeRadius(0.1);
    }
}

class BS_Missile_Entity : House
{
    protected vector m_Direction;
    protected float m_FlightSpeed;
    protected float m_SpeedMax = 120.0; 
    protected float m_Acceleration = 40.0;
    protected float m_TimeAlive = 0;
    protected float m_LastUpdateTime;
    
    // --- Guiado proporcional ---
    protected EntityAI m_HomingTarget;      // objetivo al que perseguir
    protected float m_HomingStrength = 3.5; // agresividad del guiado (grados/s escala)
    protected bool m_HomingActive = false;
    protected float m_ArmingTime = 0.3;     // segundos antes de activar guiado (evita autocolision)
    
    protected Particle m_TrailParticle;
    protected EffectSound m_FlySound;
    protected ScriptedLightBase m_Light;
    protected bool m_ExplodedClient = false;
    protected Object m_BS_Launcher;

    void BS_SetLauncher(Object launcher)
    {
        m_BS_Launcher = launcher;
        BS_AH64D_Apache_Base launcherHeli = BS_AH64D_Apache_Base.Cast(launcher);
        if (!launcherHeli) return;
        BS_DifficultyConfig launchDiff = launcherHeli.GetDiff();
        if (!launchDiff) return;
        m_SpeedMax = launchDiff.MissileMaxSpeed;
        m_Acceleration = launchDiff.MissileAcceleration;
    }

    Object BS_GetLauncher()
    {
        return m_BS_Launcher;
    }

    override void EEInit()
    {
        super.EEInit();

        if (GetGame().IsClient() || !GetGame().IsMultiplayer())
        {
            EffectSound launchSound = SEffectManager.PlaySound("Missile_Fly_SoundSet", GetPosition());
            if (launchSound) 
            {
                launchSound.SetSoundAutodestroy(true);
                launchSound.SetSoundVolume(3.0); 
            }

            m_TrailParticle = ParticleManager.GetInstance().PlayOnObject(ParticleList.BS_JD_ROCKET_TAIL, this, "0 0 0");
            
            m_FlySound = SEffectManager.PlaySoundOnObject("roadflareLoop_SoundSet", this); 
            if(m_FlySound) 
            {
                m_FlySound.SetSoundAutodestroy(true);
                m_FlySound.SetSoundVolume(3.0);
            }

            m_Light = ScriptedLightBase.CreateLight(BS_RocketLight, "0 0 0");
            m_Light.AttachOnObject(this);
        }
    }

    override void EEDelete(EntityAI parent)
    {
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(OnMissileUpdate);
        
        if (GetGame().IsClient() || !GetGame().IsMultiplayer())
        {
            if (!m_ExplodedClient && m_TimeAlive > 0.1)
            {
                CreateExplosionEffects(GetPosition());
            }

            if (m_TrailParticle) { m_TrailParticle.Stop(); m_TrailParticle = null; }
            if (m_FlySound) { m_FlySound.Stop(); m_FlySound = null; }
            if (m_Light) { m_Light.FadeOut(); m_Light = null; }
        }
        
        super.EEDelete(parent);
    }

    void Fired(vector dir, float startSpeed)
    {
        m_Direction = dir;
        m_Direction.Normalize();
        m_FlightSpeed = startSpeed;
        
        SetOrientation(m_Direction.VectorToAngles());
        m_LastUpdateTime = GetGame().GetTickTime();
        
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(OnMissileUpdate, 10, true);
    }

    void FiredAt(vector dir, float startSpeed, EntityAI target)
    {
        m_HomingTarget = target;
        
        if (target)
        {
            vector vel = GetVelocity(target);
            bool inVehicle = false;
            
            if (vel.Length() < 1.0)
            {
                EntityAI parentEnt = EntityAI.Cast(target.GetParent());
                if (parentEnt && GetVelocity(parentEnt).Length() > 5.0)
                    inVehicle = true;
            }
            else if (vel.Length() > 5.0)
            {
                inVehicle = true;
            }
            
            m_HomingActive = inVehicle;
            if (inVehicle)
                m_HomingStrength = 2.5;
            else
                m_HomingStrength = 0.0;
        }
        else
        {
            m_HomingActive = false;
        }
        
        Fired(dir, startSpeed);
    }

    void OnMissileUpdate()
    {
        float time = GetGame().GetTickTime();
        float dt = time - m_LastUpdateTime;
        m_LastUpdateTime = time;
        m_TimeAlive = m_TimeAlive + dt;

        if (m_FlightSpeed < m_SpeedMax) 
            m_FlightSpeed = m_FlightSpeed + m_Acceleration * dt;
        
        if (m_HomingActive && m_HomingTarget && m_TimeAlive >= m_ArmingTime)
        {
            EntityAI ent = m_HomingTarget;
            bool targetValid = false;
            
            SurvivorBase sb = SurvivorBase.Cast(ent);
            if (sb && sb.IsAlive() && !sb.IsUnconscious())
                targetValid = true;
            
            if (!targetValid)
            {
                CarScript car = CarScript.Cast(ent);
                if (car) targetValid = true;
            }
            
            if (targetValid)
            {
                vector targetPos = ent.GetPosition() + Vector(0, 1.0, 0);
                
                if (sb)
                {
                    EntityAI parentEnt = EntityAI.Cast(sb.GetParent());
                    if (parentEnt)
                        targetPos = parentEnt.GetPosition() + Vector(0, 1.2, 0);
                }
                
                vector toTarget = targetPos - GetPosition();
                float distToTarget = toTarget.Length();
                
                if (distToTarget > 2.0)
                {
                    toTarget = toTarget * (1.0 / distToTarget); // normalizar
                    
                    float turnRate = m_HomingStrength * dt;
                    
                    // Interpolar la direccion actual hacia el objetivo
                    float newX = m_Direction[0] + (toTarget[0] - m_Direction[0]) * turnRate;
                    float newY = m_Direction[1] + (toTarget[1] - m_Direction[1]) * turnRate;
                    float newZ = m_Direction[2] + (toTarget[2] - m_Direction[2]) * turnRate;
                    
                    m_Direction = Vector(newX, newY, newZ);
                    m_Direction.Normalize();
                    SetOrientation(m_Direction.VectorToAngles());
                }
            }
            else
            {
                // Objetivo perdido — desactivar guiado, continuar recto
                m_HomingActive = false;
                m_HomingTarget = null;
            }
        }
            
        vector currentPos = GetPosition();
        float velX = m_Direction[0] * m_FlightSpeed * dt;
        float velY = m_Direction[1] * m_FlightSpeed * dt;
        float velZ = m_Direction[2] * m_FlightSpeed * dt;
        vector nextPos = Vector(currentPos[0] + velX, currentPos[1] + velY, currentPos[2] + velZ);
        
        vector hitPos, hitNorm; 
        int hitComp; 
        set<Object> hitObjects = new set<Object>();

        bool missileHit = DayZPhysics.RaycastRV(currentPos, nextPos, hitPos, hitNorm, hitComp, hitObjects, null, this, false, false, ObjIntersectFire);
        if (missileHit && m_BS_Launcher && hitObjects.Count() > 0)
        {
            bool onlyLauncher = true;
            foreach (Object hitObj : hitObjects)
            {
                if (hitObj != m_BS_Launcher)
                    onlyLauncher = false;
            }
            if (onlyLauncher)
                missileHit = false;
        }

        if (missileHit)
        {
            #ifdef SERVER
            if (GetGame().IsServer())
            {
                string ammoType = "Explosion_40mm_Ammo";
                
                if (GetGame().ConfigIsExisting("CfgAmmo JDRPGRocket_Explosion_Ammo"))
                    ammoType = "JDRPGRocket_Explosion_Ammo";
                else if (GetGame().ConfigIsExisting("CfgAmmo Explosion_NonLethal"))
                    ammoType = "Explosion_40mm_Ammo";
                
                DamageSystem.ExplosionDamage(this, null, ammoType, hitPos, DamageType.EXPLOSION);

                array<Object> nearbyObjects = new array<Object>;
                array<CargoBase> proxyCargos = new array<CargoBase>;
                GetGame().GetObjectsAtPosition(hitPos, 12.0, nearbyObjects, proxyCargos);

                foreach (Object obj : nearbyObjects)
                {
                    WoodBase tree = WoodBase.Cast(obj);
                    if (tree && tree.IsTree() && !tree.IsDamageDestroyed())
                    {
                        tree.SetAllowDamage(true);
                        tree.DecreaseHealth("", "", tree.GetMaxHealth(), false);
                        if (tree.IsDamageDestroyed()) tree.OnTreeCutDown(this); 
                    }
                }

                SendExplosionToClients(hitPos, hitNorm);
                
                GetGame().ObjectDelete(this);
            }
            #endif
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(OnMissileUpdate);
            return;
        }

        SetPosition(nextPos);
        
        if (m_TimeAlive > 10.0) 
        {
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(OnMissileUpdate);
            #ifdef SERVER
            if (GetGame().IsServer()) GetGame().ObjectDelete(this);
            #endif
        }
    }

    void SendExplosionToClients(vector explosionPos, vector hitNormal)
    {
        BS_RPCHelper.SendToNearby_Param2Vec(this, BS_RPCIds.MISSILE_EXPLOSION, explosionPos, hitNormal, explosionPos, 800.0);
    }

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (rpc_type == BS_RPCIds.MISSILE_EXPLOSION && !GetGame().IsServer())
        {
            Param2<vector, vector> explosionParams;
            if (!ctx.Read(explosionParams)) return;
            
            CreateExplosionEffects(explosionParams.param1);
        }
    }

    void CreateExplosionEffects(vector pos)
    {
        m_ExplodedClient = true;
        
        if (!GetGame().IsClient() && GetGame().IsMultiplayer()) return;

        ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_TEST, pos);
        ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_METAL_RICOCHET, pos);
        ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_GRAVEL_RICOCHET, pos);

        EffectSound explosionSound = SEffectManager.PlaySound("Grenade_explosion_SoundSet", pos);
        if (!explosionSound)
            explosionSound = SEffectManager.PlaySound("Plastic_Explosion_SoundSet", pos);
        
        if (explosionSound) 
        {
            explosionSound.SetSoundAutodestroy(true);
            explosionSound.SetSoundVolume(2.0);
        }
        
    }

    void FadeExplosionLight(ScriptedLightBase light)
    {
        if (light)
        {
            light.FadeOut();
        }
    }
}

class BS_ExplosionLight : PointLightBase
{
    void BS_ExplosionLight()
    {
        SetFlareVisible(true);
        SetVisibleDuringDaylight(true);
        SetDiffuseColor(1.5, 0.8, 0.2);
        SetAmbientColor(1.5, 0.6, 0.1);
        SetBrightnessTo(200.0);
        SetRadiusTo(50.0);
        SetCastShadow(true);
        EnableHeatHaze(true);
        SetHeatHazePower(1.0);
        SetHeatHazeRadius(0.3);
        SetFlickerAmplitude(0.5);
        SetFlickerSpeed(5);
    }
}

class BS_Missile_Heavy_Entity : BS_Missile_Entity
{
    override void CreateExplosionEffects(vector pos)
    {
        m_ExplodedClient = true;
        
        if (!GetGame().IsClient() && GetGame().IsMultiplayer()) return;

        ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_LANDMINE, pos);
        ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_TEST, pos + Vector(0, 1, 0));
        ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_LANDMINE, pos + Vector(1, 0.5, 0));
        ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_LANDMINE, pos + Vector(-1, 0.5, 0));
        
        for (int i = 0; i < 4; i++)
        {
            float angle = i * 90.0 * Math.DEG2RAD;
            vector debrisPos = pos + Vector(Math.Sin(angle) * 2, 0.5, Math.Cos(angle) * 2);
            ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_METAL_RICOCHET, debrisPos);
        }

        EffectSound boom1 = SEffectManager.PlaySound("Plastic_Explosion_SoundSet", pos);
        if (boom1) { boom1.SetSoundAutodestroy(true); boom1.SetSoundVolume(3.0); }
        
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(PlayDelayedSound, 150, false, pos);
    }

    void PlayDelayedSound(vector pos)
    {
        EffectSound echo = SEffectManager.PlaySound("Grenade_explosion_SoundSet", pos);
        if (echo) { echo.SetSoundAutodestroy(true); echo.SetSoundVolume(1.5); }
    }
}