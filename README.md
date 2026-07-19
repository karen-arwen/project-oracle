# Project Oracle

Cozy Open World AAA — Unreal Engine 5.6, C++ e Blueprints.

Inspirado em Heartopia, Infinity Nikki, Fantasy Life, Animal Crossing, Stardew Valley e Zelda BOTW — com identidade própria.

## Estado atual

**Fase: Fundação (Prototype)** — projeto configurado, personagem third person com locomoção (walk/run/sprint, pulo, rotação suave, movimento 8 direções relativo à câmera), câmera com zoom suave e Enhanced Input completo.

## Começando

Nunca usou Unreal? Siga o passo a passo completo em **[docs/SETUP.md](docs/SETUP.md)**.

Resumo para quem já conhece:

1. Instale a Unreal Engine 5.6 e o Visual Studio 2022 (workload "Game development with C++").
2. Clique com o botão direito em `Oracle.uproject` → *Generate Visual Studio project files*.
3. Abra `Oracle.sln`, compile (Development Editor | Win64) e rode.
4. No editor, crie um mapa com um chão e aperte Play. Os controles já funcionam sem nenhum asset extra.

## Controles

| Ação | Tecla |
|---|---|
| Mover | WASD |
| Câmera | Mouse |
| Zoom | Scroll |
| Pular | Espaço |
| Sprint | Shift (segurar) |
| Andar/Correr | Ctrl (alternar) |

## Assets da Fab (não versionados)

Os packs **Stylized Village** e **Pack of tree ents** não vão para o git (licença da Epic proíbe redistribuição). Após clonar o repositório: abra o projeto → botão **Fab** no Content Browser → **Minha biblioteca** → adicione os dois packs de volta. O jogo funciona sem eles (usa formas básicas como fallback), mas fica bonito com eles.

## Documentação

- [docs/SETUP.md](docs/SETUP.md) — instalação e primeiro Play, passo a passo
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) — decisões de arquitetura e estrutura de pastas
- [ROADMAP.md](ROADMAP.md) — plano de módulos e versões
- [CHANGELOG.md](CHANGELOG.md) — histórico de mudanças
