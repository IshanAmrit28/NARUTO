import React, { useState, useMemo } from 'react';
import ReactMarkdown from 'react-markdown';
import remarkGfm from 'remark-gfm';
import { Prism as SyntaxHighlighter } from 'react-syntax-highlighter';
import { vscDarkPlus } from 'react-syntax-highlighter/dist/esm/styles/prism';
import syntaxMarkdown from '../assets/syntax.md?raw';

export default function Docs({ onTryCode }) {
  const [activeSectionIndex, setActiveSectionIndex] = useState(0);

  const sections = useMemo(() => {
    // Split the markdown by `## ` to separate major sections
    const parts = syntaxMarkdown.split(/\n## /);
    
    return parts.map((part, index) => {
      if (index === 0) {
        return {
          title: "Introduction",
          content: part.trim()
        };
      } else {
        const firstLineEnd = part.indexOf('\n');
        const titleLine = part.substring(0, firstLineEnd).trim();
        // Restore the "## " header for proper rendering in markdown
        const content = `## ${part}`; 
        
        // Clean title (remove numbers like "1. ")
        const cleanTitle = titleLine.replace(/^\d+\.\s*/, '');
        
        return {
          title: cleanTitle,
          content: content.trim()
        };
      }
    });
  }, []);

  const activeSection = sections[activeSectionIndex];

  return (
    <div className="docs-container">
      <div className="docs-sidebar">
        <h3 className="sidebar-title">Naruto Syntax</h3>
        <ul className="sidebar-list">
          {sections.map((section, index) => (
            <li 
              key={index} 
              className={`sidebar-item ${activeSectionIndex === index ? 'active' : ''}`}
              onClick={() => setActiveSectionIndex(index)}
            >
              {section.title}
            </li>
          ))}
        </ul>
      </div>
      
      <div className="docs-content">
        <div className="docs-reading-pane">
          <ReactMarkdown
            remarkPlugins={[remarkGfm]}
            components={{
              code({ node, inline, className, children, ...props }) {
                const match = /language-(\w+)/.exec(className || '');
                const codeString = String(children).replace(/\n$/, '');
                
                if (!inline && match) {
                  return (
                    <div className="code-block-wrapper">
                      <div className="code-block-header">
                        <span className="code-lang">{match[1]}</span>
                        <button 
                          className="try-btn" 
                          onClick={() => onTryCode(codeString)}
                        >
                          Try it Yourself &raquo;
                        </button>
                      </div>
                      <SyntaxHighlighter
                        style={vscDarkPlus}
                        language={match[1]}
                        PreTag="div"
                        className="syntax-highlighter-pre"
                        {...props}
                      >
                        {codeString}
                      </SyntaxHighlighter>
                    </div>
                  );
                }
                return (
                  <code className={`inline-code ${className || ''}`} {...props}>
                    {children}
                  </code>
                );
              }
            }}
          >
            {activeSection?.content}
          </ReactMarkdown>
        </div>
      </div>
    </div>
  );
}
