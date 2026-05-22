import React, { useState } from 'react';
import Introduction from './docs_pages/Introduction';
import VariablesAndTypes from './docs_pages/VariablesAndTypes';
import InputOutput from './docs_pages/InputOutput';
import ControlFlow from './docs_pages/ControlFlow';
import DataStructures from './docs_pages/DataStructures';
import Functions from './docs_pages/Functions';
import OOP from './docs_pages/OOP';

export default function Docs({ onTryCode }) {
  const [activeSectionId, setActiveSectionId] = useState('intro');

  const sections = [
    { id: 'intro', title: 'Introduction', component: Introduction },
    { id: 'variables', title: 'Variables & Data Types', component: VariablesAndTypes },
    { id: 'io', title: 'Input & Output', component: InputOutput },
    { id: 'control_flow', title: 'Control Flow & Operators', component: ControlFlow },
    { id: 'data_structures', title: 'Arrays & Structs', component: DataStructures },
    { id: 'functions', title: 'Functions', component: Functions },
    { id: 'oop', title: 'Object-Oriented Programming', component: OOP }
  ];

  const renderActiveSection = () => {
    const activeSection = sections.find(s => s.id === activeSectionId);
    if (!activeSection) return null;
    
    const Component = activeSection.component;
    return <Component onTryCode={onTryCode} />;
  };

  return (
    <div className="docs-container">
      <div className="docs-sidebar">
        <h3 className="sidebar-title">Naruto Syntax</h3>
        <ul className="sidebar-list">
          {sections.map((section) => (
            <li 
              key={section.id} 
              className={`sidebar-item ${activeSectionId === section.id ? 'active' : ''}`}
              onClick={() => setActiveSectionId(section.id)}
            >
              {section.title}
            </li>
          ))}
        </ul>
      </div>
      
      <div className="docs-content">
        <div className="docs-reading-pane">
          {renderActiveSection()}
        </div>
      </div>
    </div>
  );
}
