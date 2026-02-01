import logo from './logo.svg';
import React, { useEffect } from 'react';
import { BrowserRouter, Routes, Route } from 'react-router-dom';
import { FiSettings } from 'react-icons/fi';
import { TooltipComponent } from '@syncfusion/ej2-react-popups'

import { Navbar, Footer, Sidebar, ThemeSettings } from './components';
import { ECmmrc, Orders, Calendar, Employees, Companies, Stacked, Pyramid, Customers, Kanban, Line, Area, Bar, Pie, Financial, ColorPicker, ColorMapping, Editor } from './pages';

import './App.css';

import { useStateContext  } from './contexts/ContextProvider';


function App() {
  const { activeMenu, themeSettings, setThemeSettings, currentColor, currentMode } = useStateContext();

  return ( 
    <div className={ currentMode === 'Dark' ? 'dark' : ''}>
      <BrowserRouter>
        <div className="flex relative dark:bg-main-dark-bg">

          { /* ---- [1] DOWN RIGHT: Button (icon) (Settings) ---- */ }
          <div className="fixed right-4 bottom-4" stxle={{ zIndex: '1000' }}>
            <TooltipComponent content="Settings" position="Top">
              <button type="button" className="text-3xl p-3 hover:drop-shadow-xl hover:bg-light-gray text-white"
                      style={{ background: currentColor, borderRadius: '50%'}}
                      onClick={() => setThemeSettings(true)} >
                <FiSettings />
              </button>
            </TooltipComponent>
          </div>

          { /* ---- [2] LEFT: Sidebar component ---- */ }
          { /* w-72 (width), dark:bg-secondary-dark-bg (on dark mode) - shaddow below*/ }
          { activeMenu ? (
            <div className="w-72 fixed sidebar dark:bg-secondary-dark-bg bg-white"> 
              <Sidebar />
            </div>
          ) : ( 
            <div className="w-0 dark:bg-secondary-dark-bg">
              <Sidebar />
            </div>
          )}

          { /* ---- [3] TOP: Top Navbar ----
               Note className is dynamic block of code in {}, not. Also check on google: w-full ""*/ }
          <div className={
            // JS Code for string concatenation
            `dark:bg-main-bg bg-main-bg min-h-screen w-full ${activeMenu ? 'md:ml-72' : 'flex-2'}`
            }>
              <div className="fixed md:static bg-main-bg dark:bg-main-dark-bg navbar w-full"> 
                <Navbar />
              </div>


          { /* ----==== [4] CENTAR: MAIN DIV for Routing ====---- */ }
          <div>
            { themeSettings && <ThemeSettings /> }

            <Routes>
              { /* Dashboard:
                   element to render - can be simple string (or complex component (page))
                   ecommerce path renders ECommerce component
                   We will access this via left active bar */ }
              <Route path="/" element={ <ECmmrc/> } />
              <Route path="/ecommerce" element={ <ECmmrc/> } />

              { /* Pages */ }
              <Route path="/orders" element={ <Orders/> } />
              <Route path="/employees" element={ <Employees/> } />
              <Route path="/companies" element={ <Companies/> } />
              <Route path="/customers" element={ <Customers/> } />

              { /* Apps */ }
              <Route path="/kanban" element={ <Kanban/> } />
              <Route path="/editor" element={ <Editor/> } />
              <Route path="/calendar" element={ <Calendar/> } />
              <Route path="/color-picker" element={ <ColorPicker/> } />

              { /* Charts */ }
              <Route path="/line" element={ <Line/> } />
              <Route path="/area" element={ <Area/> } />
              <Route path="/bar" element={ <Bar/> } />
              <Route path="/pie" element={ <Pie/> } />
              <Route path="/financial" element={ <Financial/> } />
              <Route path="/color-mapping" element={ <ColorMapping/> } />
              <Route path="/pyramid" element={ <Pyramid/> } />
              <Route path="/stacked" element={ <Stacked/> } />           
            </Routes>
          </div>

          </div>
        </div>
      </BrowserRouter>
    </div>
  );
}

export default App;
