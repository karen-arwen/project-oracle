# Arquitetura — Project Oracle

## Princípio central

O projeto vai crescer por anos e ganhar dezenas de sistemas. Toda decisão desta fundação serve a uma regra: **cada sistema novo deve entrar sem tocar nos existentes**. Isso se traduz em três práticas: classes finas que coordenam, componentes que implementam, e dados (Data Assets) que configuram.

## Estrutura de pastas

```
Oracle/
├── Oracle.uproject
├── Config/                  # .ini do projeto (engine, game, input)
├── Content/                 # Assets (criados no editor)
│   └── Oracle/              # TUDO nosso vive sob este prefixo
│       ├── Characters/
│       ├── Input/           # IA_*, IMC_*, IC_* (Data Assets de input)
│       ├── Maps/            # L_Sandbox, futuros níveis
│       └── UI/
├── Source/
│   ├── Oracle.Target.cs
│   ├── OracleEditor.Target.cs
│   └── Oracle/              # Módulo principal (runtime)
│       ├── Core/            # GameMode, PlayerController, futuros Subsystems
│       ├── Character/       # Character + MovementComponent
│       ├── Camera/          # Controle de câmera
│       └── Input/           # InputConfig + defaults de runtime
└── docs/
```

Convenções de nome: classes `Oracle*` (AOracleCharacter, UOracleCameraComponent); assets com prefixo por tipo (`BP_`, `IA_`, `IMC_`, `L_`, `SKM_`, `ABP_`); mapas e assets nossos sempre dentro de `Content/Oracle/` para nunca misturar com packs importados.

## Decisões e porquês

**Um módulo agora, vários depois.** Tudo vive no módulo `Oracle`, organizado por pastas que espelham os futuros módulos (Core, Character, Camera, Input). Quando um domínio crescer (ex.: Building, Farming), a pasta vira um módulo UBT próprio com dependências explícitas. Criar 20 módulos no dia 1 seria burocracia sem benefício.

**Character fino, componentes gordos.** `AOracleCharacter` só coordena: cria componentes e roteia input. Locomoção vive no `UOracleCharacterMovementComponent` (velocidades por marcha, futuros modos custom — escalada, planador — via `MOVE_Custom`). Câmera vive no `UOracleCameraComponent`. Sistemas futuros (interação, inventário, habilidades) entram como novos componentes, não como métodos no Character.

**Por que estender o CharacterMovementComponent em vez de fazer movimento próprio:** o CMC da engine já resolve rede (prediction/replay), física de chão, degraus e encroachment — anos de trabalho testado em produção. Escalada e planador serão modos custom DENTRO dele, o caminho usado por jogos AAA reais.

**Input como dados, com fallback em código.** O código só conhece `UInputAction`s agrupadas no `UOracleInputConfig` (Data Asset) — nunca teclas. Mapeamento tecla→ação vive no `UInputMappingContext`. Se os assets não existirem, `FOracleInputDefaults` constrói tudo em runtime: o projeto compila e joga sem nenhum clique no editor. Quando você criar os assets no editor (workflow definitivo), basta preenchê-los no `OraclePlayerController` e o fallback se desliga sozinho. Contextos futuros (UI, veículo, Photo Mode) entram com prioridades maiores, permitindo trocar todo o esquema de controle empilhando/removendo contextos.

**GameMode/Controller enxutos.** O `OraclePlayerController` tem uma responsabilidade: registrar Mapping Contexts. O bind das ações fica no Pawn — quando houver montarias e veículos, cada Pawn possuído fará seus próprios binds e o esquema muda automaticamente ao trocar de Pawn.

**Renderização AAA desde o config.** Lumen (GI + reflexos), Nanite, Virtual Shadow Maps, TSR e DX12 já ativos em `DefaultEngine.ini` — mudar isso depois invalida iluminação e custa retrabalho.

## Onde cada sistema futuro vai viver

| Sistema | Onde | Mecanismo |
|---|---|---|
| Escalada / Planador | `Character/` (CMC) | `MOVE_Custom` + `EOracleCustomMovement` (enum já existe) |
| Natação | `Character/` (CMC) | `MOVE_Swimming` nativo + volumes de água |
| Interação (colher, pescar, falar) | novo `Interaction/` | Componente + interface `IOracleInteractable` |
| Habilidades / stats de roupas | novo `Abilities/` | Gameplay Ability System (plugin entra nessa hora) |
| Inventário / coleções | novo `Inventory/` | Componente + Data Assets de itens |
| Save | `Core/` | `UGameInstanceSubsystem` + SaveGame versionado |
| Tempo/clima/estações | `Core/` ou `World/` | `UWorldSubsystem` |
| Photo Mode | `Camera/` | Assume o `UOracleCameraComponent` |
| UI | novo `UI/` | Common UI, contexto de input próprio |
| Mundo aberto | Content + `World/` | World Partition (ativar ao criar o mapa grande) |

## Padrões de código

SOLID aplicado com pragmatismo: responsabilidade única por classe, componentes para composição, dados para configuração. `TObjectPtr` em UPROPERTYs, `check`/logs (`LogOracle`) em pré-condições, comentários apenas onde a intenção não é óbvia. Tabs, chaves de Allman, prefixos padrão da engine (A/U/F/E) — estilo oficial Epic.
