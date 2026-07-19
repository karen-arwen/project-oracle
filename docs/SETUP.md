# Setup — do zero ao primeiro Play

Guia para quem nunca usou Unreal Engine. Tempo estimado: 1–2 horas (a maior parte é download).

## 1. Instalar as ferramentas

### Visual Studio 2022 (compilador C++)
1. Baixe o **Visual Studio 2022 Community** (gratuito): https://visualstudio.microsoft.com/pt-br/
2. No instalador, marque a workload **"Desenvolvimento de jogos com C++"** (Game development with C++).
3. Dentro dela, confirme que estão marcados: *Unreal Engine installer support* e o *Windows 10/11 SDK*.

### Unreal Engine 5.6
1. Baixe o **Epic Games Launcher**: https://www.unrealengine.com/download
2. No launcher, aba **Unreal Engine → Biblioteca → "+"** e instale a versão **5.6**.
3. (Opcional, economiza disco) Em Options da instalação, desmarque plataformas que não vai usar (Android, iOS, Linux).

### Git + Git LFS
1. Instale o Git: https://git-scm.com/downloads
2. Instale o Git LFS: https://git-lfs.com/
3. Abra um terminal na pasta do projeto e rode uma única vez:
   ```
   git lfs install
   git init
   git add .
   git commit -m "Fundacao do projeto"
   ```

## 2. Compilar o projeto

1. Na pasta do projeto, clique com o **botão direito** em `Oracle.uproject` → **Generate Visual Studio project files**.
   - Se a opção não aparecer: "Mostrar mais opções" (Win11), ou abra o .uproject com duplo clique e aceite quando a engine oferecer compilar.
2. Abra o arquivo `Oracle.sln` gerado (no Visual Studio).
3. No topo, confirme a configuração **Development Editor** e plataforma **Win64**.
4. Menu **Build → Build Solution** (ou Ctrl+Shift+B). A primeira compilação demora vários minutos.
5. Ao terminar sem erros, aperte **F5** (ou duplo clique no `Oracle.uproject`) para abrir o editor.

## 3. Criar o mapa de teste (só na primeira vez)

O código está pronto, mas um mapa (level) é um asset — você o cria no editor em 2 minutos:

1. **File → New Level → Basic** (vem com chão, luz e céu).
2. **File → Save Current Level As...** → crie a pasta `Content/Oracle/Maps` e salve como **`L_Sandbox`**.
3. **Edit → Project Settings → Maps & Modes**: defina *Editor Startup Map* e *Game Default Map* como `L_Sandbox`.
   (Confirme na mesma tela que *Default GameMode* é `OracleGameMode` — já vem do config.)
4. Aperte **Play** (botão ▶ ou Alt+P).

Pronto: WASD move, mouse gira a câmera, scroll dá zoom, Espaço pula, Shift dá sprint, Ctrl alterna caminhada. Tudo isso funciona **sem criar nenhum asset de input** — o código gera as teclas padrão em runtime.

O personagem ainda é uma cápsula invisível com câmera — é o esperado nesta fase. Malha e animações são o próximo módulo (ver ROADMAP).

## 4. (Opcional) Ver um boneco em vez da cápsula

1. No editor: **Content Browser → Add → Add Feature or Content Pack → Third Person** → Add to Project.
2. Crie um Blueprint filho de `OracleCharacter` (clique direito em `OracleCharacter` no C++ Classes → *Create Blueprint class based on...*), salve em `Content/Oracle/Characters/BP_OracleCharacter`.
3. No Blueprint, no componente **Mesh**: escolha o Skeletal Mesh `SKM_Quinn` e a Anim Class `ABP_Quinn`; ajuste a posição do mesh (Z ≈ -90, rotação Z = -90).
4. Em **Project Settings → Maps & Modes**, ou num GameMode Blueprint, defina o *Default Pawn Class* como `BP_OracleCharacter`.

## Problemas comuns

| Sintoma | Causa provável | Solução |
|---|---|---|
| "Generate project files" não aparece | Engine não registrada | Abra o Epic Launcher uma vez; ou repare a instalação da engine |
| Erro de compilação sobre SDK | Windows SDK ausente | Reabra o instalador do VS e adicione o Windows 10/11 SDK |
| Editor abre mas Play não move | Mapa sem GameMode correto | Confira Project Settings → Maps & Modes → Default GameMode = OracleGameMode |
| Personagem cai infinitamente | Mapa sem chão | Use New Level → Basic, não Empty |
