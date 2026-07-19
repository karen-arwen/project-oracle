# Changelog

Formato baseado em [Keep a Changelog](https://keepachangelog.com/pt-BR/1.1.0/).

## [0.4.0] - 2026-07-19 — Ambiente visual dinâmico

### Adicionado
- `AOracleEnvironment` (spawnado pelo GameMode): SkyAtmosphere, nuvens volumétricas, névoa com godrays (volumetric fog), SkyLight em tempo real, PostProcess unbound com grading vibrante (bloom 0.7, saturação +14%, vinheta, exposure bias).
- Ciclo de luz completo: sol laranja no amanhecer/entardecer, branco-quente ao meio-dia, noite azulada com névoa densa — dirigido pelo TimeSubsystem.
- Remoção automática do rig de céu duplicado do mapa (configurável).
- `docs/ART_DIRECTION.md`: direção de arte + packs do Fab recomendados e passos de aplicação.

### Alterado
- `UOracleTimeSubsystem`: fallback do sol cede controle ao Environment via `SetSunController`.

## [0.3.0] - 2026-07-19 — Loop cozy completo (Módulos 4–10)

### Adicionado
- **Interação**: interface `IOracleInteractable` (C++/BP) + `UOracleInteractionComponent` (busca por proximidade, prompt no HUD, tecla E).
- **Itens**: `UOracleItemDefinition` (Data Asset — nome, categoria, pilha, malha de mundo, semente→cultivo).
- **Inventário**: `UOracleInventoryComponent` com pilhas, delegate de mudança.
- **Coleta**: `AOracleGatherable` — dá itens, some e reaparece; feedback null-safe.
- **Tempo**: `UOracleTimeSubsystem` — relógio (dia de 20min), dias, eventos OnDay/OnHourChanged, rotação automática do sol da cena.
- **Fazenda**: `UOracleCropDefinition` + `AOracleFarmPlot` — plantar/regar/crescer por dias/colher, broto cresce visualmente.
- **Save/Load**: `UOracleSaveGame` (versionado) + `UOracleSaveSubsystem` — F5/F9; salva jogador, tempo, inventário, canteiros e móveis.
- **Decoração**: `UOraclePlacementComponent` — modo construção (B) com fantasma, grid snap 50cm, rotação 45° (R), tint verde/vermelho; `AOraclePlacedProp` guardável com E.
- **HUD**: `AOracleHUD` em Canvas — relógio, prompt, mochila, dicas (será substituído por Common UI).
- **Mundo demo**: `AOracleDemoWorld` autogerado pelo GameMode — itens de teste, arbustos, troncos e fazendinha; loop completo jogável sem nenhum asset.
- Novas teclas: E interagir, B decorar, clique posicionar, R girar, F5/F9 save/load.

## [0.2.0] - 2026-07-18 — Game feel + personagem visual (código)

### Adicionado
- Coyote time e jump buffer configuráveis no Character.
- Pulo variável (segurar Espaço pula mais alto) e gravidade 1.5x para pulo com peso.
- Kick de FOV no sprint com interpolação (só com velocidade real).
- `UOracleLandingShake`: camera shake de pouso 100% em C++, escalado pela queda.
- Ganchos null-safe de feedback: som/poeira de pouso, som de passos (`PlayFootstep` p/ Anim Notify).
- `UOracleAnimInstance`: base C++ do Animation Blueprint (GroundSpeed, bShouldMove, bIsFalling, Gait).
- Dependência do módulo Niagara.
- `docs/GUIA_PERSONAGEM_VISUAL.md`: passo a passo do editor (mesh, ABP, notifies, VFX).

### Alterado
- `UOracleCameraComponent`: `Initialize(SpringArm, Camera)` substitui `SetSpringArm`.

## [0.1.0] - 2026-07-18 — Fundação

### Adicionado
- Projeto UE 5.6 com módulo C++ `Oracle`, targets Game e Editor.
- Configs: Lumen, Nanite, Virtual Shadow Maps, DX12, física assíncrona.
- Git: `.gitignore` UE5, `.gitattributes` com Git LFS para assets binários.
- `AOracleGameMode` e `AOraclePlayerController` (registro de Mapping Contexts).
- `AOracleCharacter`: movimento 8 direções relativo à câmera, pulo, rotação suave.
- `UOracleCharacterMovementComponent`: marchas Walk/Run/Sprint com velocidades configuráveis; enums preparados para escalada, planador, montarias.
- `UOracleCameraComponent`: câmera third person com SpringArm, lag e zoom interpolado.
- Enhanced Input: `UOracleInputConfig` (Data Asset) + fallback de teclas em runtime (jogável sem criar assets).
- Documentação: README, SETUP, ARCHITECTURE, ROADMAP.
