# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### C++ Backend (Mod-Inv)

```bash
# Build from sw/Mod-Inv directory
cd sw/Mod-Inv
make all           # Build main binary -> ../../bins/Mod-Inv
make clean         # Clean build artifacts

# Run the application
../../bins/Mod-Inv

# Unit tests
make unitTestNet       # Build networking tests -> bins/Mod-Inv_UT/Mod-Inv_UT_Net
make unitTestSrvX      # Build ServiceX tests -> bins/Mod-Inv_UT/Mod-Inv_UT_Srv_X
make unitTestSLS       # Build Linux system tests -> bins/Mod-Inv_UT/Mod-Inv_UT_SLS
make cleanAllUnitTests # Clean all test artifacts

# Run a specific unit test
../../bins/Mod-Inv_UT/Mod-Inv_UT_Net
```

### React Dashboard

```bash
cd webdev/dashboard_0
npm install
npm start    # Dev server on localhost:3000
npm test     # Run tests
npm run build
```

## Architecture

### C++ Service Architecture (sw/Mod-Inv)

The backend uses a **Factory pattern with dependency injection**. Services are registered and instantiated via a singleton Factory that reads configuration from a database.

**Key components:**

- **Factory** (`services/Factory/`) - Singleton that manages service registration and instantiation using `REGISTER_CLASS(ClassName)` macro
- **ObjectsManager** (`services/Container/`) - IoC container storing service instances
- **BrokerSrv** - Application entry point, calls `preInit()` to bootstrap
- **MasterSrv** - Orchestrates investment development services, handles CLI commands via `cmdSecondWord()`
- **ClientServerSrv** - Unix domain socket communication (`/tmp/DemoSocket`), provides `TRACE()` logging
- **HTTPSProxy/Networking** - HTTPS client using ASIO + OpenSSL

**Service interfaces follow pattern:** `services/<Name>/if/` for interface, `services/<Name>/inc/` for headers, `services/<Name>/src/` for implementation

### Adding a New Service

1. Copy `services/ServiceX/` folder and rename files
2. Add folder path to `CODE_DIRS` in Makefile
3. In `Factory.h`:
   - Add `REGISTER_CLASS(Services::YourService)`
   - Include your header files
   - Add interface comparison in `createInstances()`
4. Add container in ObjectsManager

### External Libraries (libs/)

- **ASIO 1.22.1** - Async I/O (header-only)
- **Boost** - bind, iterators
- **OpenSSL** - libssl.a, libcrypto.a in `libs/sslLibs/`
- **Google Test/Mock** - in `libs/unitTest/`
- **RapidJSON** - JSON parsing

### React Dashboard (webdev/dashboard_0)

React 18 app with:
- **Syncfusion EJ2** - Charts, grids, calendar, kanban components
- **Recharts** - Additional charting
- **Tailwind CSS** - Styling
- **React Router** - Navigation

Key directories: `src/pages/` (page components), `src/components/` (reusable UI), `src/contexts/` (state management)

## Key Macros

```cpp
FACTORY                    // Access Factory singleton
REGISTER_CLASS(ClassName)  // Register class constructor in Factory
LOG                        // Logging prefix with file/line info
```
