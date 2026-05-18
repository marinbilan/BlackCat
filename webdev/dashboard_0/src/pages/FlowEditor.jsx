import React, { useState, useCallback, useRef, useMemo } from 'react';
import {
  ReactFlow,
  addEdge,
  applyNodeChanges,
  applyEdgeChanges,
  Background,
  Controls,
  MiniMap,
  Handle,
  Position,
} from '@xyflow/react';
import '@xyflow/react/dist/style.css';
import { Header } from '../components';

// ── Block node (rectangle) ────────────────────────────────────────
const BlockNode = ({ data }) => {
  let bg = data.baseColor || '#1D4ED8';
  if (data.active) bg = '#16a34a';
  else if (data.done) bg = '#6b7280';

  return (
    <div style={{
      background: bg,
      color: '#fff',
      borderRadius: 8,
      padding: '10px 18px',
      minWidth: 130,
      textAlign: 'center',
      fontFamily: 'Inter, sans-serif',
      fontSize: 13,
      fontWeight: 600,
      boxShadow: '0 2px 8px rgba(0,0,0,0.18)',
      transition: 'background 0.4s ease',
    }}>
      <Handle type="target" position={Position.Left} />
      <div style={{ marginBottom: 6 }}>{data.label}</div>
      <input
        type="number"
        min="1"
        max="60"
        value={data.duration}
        onChange={(e) => data.onDurationChange(Number(e.target.value))}
        style={{
          width: 58,
          textAlign: 'center',
          borderRadius: 4,
          border: 'none',
          padding: '2px 6px',
          color: '#111827',
          fontSize: 12,
          fontWeight: 500,
        }}
        onClick={(e) => e.stopPropagation()}
        onPointerDown={(e) => e.stopPropagation()}
      />
      <div style={{ fontSize: 10, opacity: 0.75, marginTop: 2 }}>sec</div>
      <Handle type="source" position={Position.Right} />
    </div>
  );
};

// ── Condition node (diamond) ──────────────────────────────────────
const ConditionNode = ({ data }) => {
  let bg = data.baseColor || '#d97706';
  if (data.active) bg = '#16a34a';
  else if (data.done) bg = '#6b7280';

  return (
    <div style={{
      position: 'relative',
      width: 120,
      height: 120,
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'center',
    }}>
      <Handle type="target" position={Position.Left} />
      {/* Rotated square → diamond shape */}
      <div style={{
        position: 'absolute',
        width: 88,
        height: 88,
        background: bg,
        transform: 'rotate(45deg)',
        borderRadius: 8,
        boxShadow: '0 2px 8px rgba(0,0,0,0.18)',
        transition: 'background 0.4s ease',
      }} />
      {/* Content sits on top, unrotated */}
      <div style={{
        position: 'relative',
        zIndex: 1,
        color: '#fff',
        textAlign: 'center',
        fontFamily: 'Inter, sans-serif',
        fontWeight: 600,
        fontSize: 11,
        lineHeight: 1.3,
      }}>
        <div style={{ marginBottom: 4 }}>{data.label}</div>
        <input
          type="number"
          min="1"
          max="60"
          value={data.duration}
          onChange={(e) => data.onDurationChange(Number(e.target.value))}
          style={{
            width: 46,
            textAlign: 'center',
            borderRadius: 4,
            border: 'none',
            padding: '2px 4px',
            color: '#111827',
            fontSize: 11,
            fontWeight: 500,
          }}
          onClick={(e) => e.stopPropagation()}
          onPointerDown={(e) => e.stopPropagation()}
        />
        <div style={{ fontSize: 9, opacity: 0.8, marginTop: 1 }}>sec</div>
      </div>
      <Handle type="source" position={Position.Right} />
    </div>
  );
};

const nodeTypes = { block: BlockNode, condition: ConditionNode };

// ── Node palette (right sidebar) ──────────────────────────────────
const PALETTE_ITEMS = [
  {
    type: 'block',
    label: 'Block',
    baseColor: '#1D4ED8',
    preview: (
      <div style={{
        background: '#1D4ED8',
        color: '#fff',
        borderRadius: 6,
        padding: '6px 14px',
        fontSize: 12,
        fontWeight: 600,
        textAlign: 'center',
      }}>
        Block
      </div>
    ),
  },
  {
    type: 'condition',
    label: 'Condition',
    baseColor: '#d97706',
    preview: (
      <div style={{
        position: 'relative',
        height: 60,
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
      }}>
        <div style={{
          position: 'absolute',
          width: 44,
          height: 44,
          background: '#d97706',
          transform: 'rotate(45deg)',
          borderRadius: 5,
        }} />
        <span style={{
          position: 'relative',
          zIndex: 1,
          color: '#fff',
          fontSize: 11,
          fontWeight: 600,
        }}>Cond.</span>
      </div>
    ),
  },
];

const NodePalette = () => {
  const onDragStart = (event, item) => {
    event.dataTransfer.setData('rf/type',  item.type);
    event.dataTransfer.setData('rf/label', item.label);
    event.dataTransfer.setData('rf/color', item.baseColor);
    event.dataTransfer.effectAllowed = 'move';
  };

  return (
    <div style={{
      width: 150,
      background: '#f9fafb',
      border: '1px solid #e5e7eb',
      borderRadius: 12,
      padding: '14px 10px',
      display: 'flex',
      flexDirection: 'column',
      gap: 14,
      flexShrink: 0,
    }}>
      <div style={{
        fontSize: 11,
        fontWeight: 700,
        color: '#6b7280',
        textTransform: 'uppercase',
        letterSpacing: '0.06em',
        textAlign: 'center',
      }}>
        Nodes
      </div>
      {PALETTE_ITEMS.map((item) => (
        <div
          key={item.type}
          draggable
          onDragStart={(e) => onDragStart(e, item)}
          title={`Drag to add a ${item.label} node`}
          style={{
            cursor: 'grab',
            userSelect: 'none',
            padding: '4px 4px',
            borderRadius: 8,
            border: '1px dashed #d1d5db',
            background: '#fff',
            transition: 'box-shadow 0.15s',
          }}
          onMouseEnter={(e) => (e.currentTarget.style.boxShadow = '0 2px 8px rgba(0,0,0,0.12)')}
          onMouseLeave={(e) => (e.currentTarget.style.boxShadow = 'none')}
        >
          {item.preview}
        </div>
      ))}
      <div style={{ fontSize: 10, color: '#9ca3af', textAlign: 'center', marginTop: 'auto' }}>
        Drag onto canvas
      </div>
    </div>
  );
};

// ── Initial data ──────────────────────────────────────────────────
const initialNodes = [
  { id: '1', type: 'block',     position: { x: 80,  y: 160 }, data: { label: 'Input',     baseColor: '#15803d', duration: 3, active: false, done: false } },
  { id: '2', type: 'block',     position: { x: 310, y: 70  }, data: { label: 'Process A', baseColor: '#1D4ED8', duration: 2, active: false, done: false } },
  { id: '3', type: 'condition', position: { x: 310, y: 230 }, data: { label: 'Check',     baseColor: '#d97706', duration: 3, active: false, done: false } },
  { id: '4', type: 'block',     position: { x: 560, y: 160 }, data: { label: 'Output',    baseColor: '#b91c1c', duration: 2, active: false, done: false } },
];

const initialEdges = [
  { id: 'e1-2', source: '1', target: '2', animated: true },
  { id: 'e1-3', source: '1', target: '3', animated: true },
];

// ── Component ─────────────────────────────────────────────────────
const FlowEditor = () => {
  const [nodes, setNodes] = useState(initialNodes);
  const [edges, setEdges] = useState(initialEdges);
  const [playing, setPlaying]   = useState(false);
  const [rfInstance, setRfInstance] = useState(null);

  const timeoutsRef = useRef([]);
  const doneRef     = useRef(new Set());
  const wrapperRef  = useRef(null);

  // Inject onDurationChange handlers without mutating raw state
  const nodesWithHandlers = useMemo(() =>
    nodes.map((n) => ({
      ...n,
      data: {
        ...n.data,
        onDurationChange: (val) =>
          setNodes((nds) =>
            nds.map((nd) =>
              nd.id === n.id
                ? { ...nd, data: { ...nd.data, duration: Math.max(1, val) } }
                : nd
            )
          ),
      },
    })),
    [nodes]
  );

  const onNodesChange = useCallback(
    (changes) => setNodes((nds) => applyNodeChanges(changes, nds)), []
  );
  const onEdgesChange = useCallback(
    (changes) => setEdges((eds) => applyEdgeChanges(changes, eds)), []
  );
  const onConnect = useCallback(
    (conn) => setEdges((eds) => addEdge({ ...conn, animated: true }, eds)), []
  );

  // ── Drag-and-drop from palette ──────────────────────────────────
  const onDragOver = useCallback((event) => {
    event.preventDefault();
    event.dataTransfer.dropEffect = 'move';
  }, []);

  const onDrop = useCallback((event) => {
    event.preventDefault();
    if (!rfInstance) return;

    const type  = event.dataTransfer.getData('rf/type');
    const label = event.dataTransfer.getData('rf/label');
    const color = event.dataTransfer.getData('rf/color');
    if (!type) return;

    const position = rfInstance.screenToFlowPosition({
      x: event.clientX,
      y: event.clientY,
    });

    const newNode = {
      id: `node-${Date.now()}`,
      type,
      position,
      data: { label, baseColor: color, duration: 2, active: false, done: false },
    };
    setNodes((nds) => [...nds, newNode]);
  }, [rfInstance]);

  // ── Reset ───────────────────────────────────────────────────────
  const stopFlow = useCallback(() => {
    timeoutsRef.current.forEach(clearTimeout);
    timeoutsRef.current = [];
    doneRef.current = new Set();
    setPlaying(false);
    setNodes((nds) =>
      nds.map((n) => ({ ...n, data: { ...n.data, active: false, done: false } }))
    );
  }, []);

  // ── Play ────────────────────────────────────────────────────────
  const playFlow = useCallback(() => {
    timeoutsRef.current.forEach(clearTimeout);
    timeoutsRef.current = [];
    doneRef.current = new Set();
    setPlaying(true);

    const snapNodes = nodes;
    const snapEdges = edges;

    setNodes((nds) =>
      nds.map((n) => ({ ...n, data: { ...n.data, active: false, done: false } }))
    );

    const inMap  = {};
    const outMap = {};
    snapNodes.forEach((n) => { inMap[n.id] = new Set(); outMap[n.id] = []; });
    snapEdges.forEach((e) => {
      if (inMap[e.target])  inMap[e.target].add(e.source);
      if (outMap[e.source]) outMap[e.source].push(e.target);
    });

    const durMap = {};
    snapNodes.forEach((n) => { durMap[n.id] = (n.data.duration || 1) * 1000; });

    const done = doneRef.current;
    const totalNodes = snapNodes.length;

    const activate = (nodeId) => {
      setNodes((nds) =>
        nds.map((n) =>
          n.id === nodeId ? { ...n, data: { ...n.data, active: true, done: false } } : n
        )
      );

      const t = setTimeout(() => {
        done.add(nodeId);
        setNodes((nds) =>
          nds.map((n) =>
            n.id === nodeId ? { ...n, data: { ...n.data, active: false, done: true } } : n
          )
        );

        outMap[nodeId].forEach((targetId) => {
          const allPredsDone = [...inMap[targetId]].every((p) => done.has(p));
          if (allPredsDone) activate(targetId);
        });

        if (done.size === totalNodes) setPlaying(false);
      }, durMap[nodeId]);

      timeoutsRef.current.push(t);
    };

    snapNodes.forEach((n) => {
      if (inMap[n.id].size === 0) activate(n.id);
    });
  }, [nodes, edges]);

  // ── Render ──────────────────────────────────────────────────────
  return (
    <div className="m-2 md:m-10 p-2 md:p-10 bg-white rounded-3xl">
      <Header category="App" title="Flow Editor" />

      {/* Controls bar */}
      <div style={{ display: 'flex', gap: 10, marginBottom: 12 }}>
        <button
          onClick={playFlow}
          disabled={playing}
          style={{
            padding: '8px 24px',
            borderRadius: 8,
            border: 'none',
            background: playing ? '#9ca3af' : '#16a34a',
            color: '#fff',
            fontWeight: 700,
            fontSize: 14,
            cursor: playing ? 'not-allowed' : 'pointer',
          }}
        >
          ▶ Play
        </button>
        <button
          onClick={stopFlow}
          style={{
            padding: '8px 24px',
            borderRadius: 8,
            border: 'none',
            background: '#b91c1c',
            color: '#fff',
            fontWeight: 700,
            fontSize: 14,
            cursor: 'pointer',
          }}
        >
          ■ Reset
        </button>
      </div>

      {/* Canvas + Palette */}
      <div style={{ display: 'flex', gap: 12, height: '68vh' }}>
        <div
          ref={wrapperRef}
          style={{ flex: 1, border: '1px solid #e5e7eb', borderRadius: 12, overflow: 'hidden' }}
          onDrop={onDrop}
          onDragOver={onDragOver}
        >
          <ReactFlow
            nodes={nodesWithHandlers}
            edges={edges}
            onNodesChange={onNodesChange}
            onEdgesChange={onEdgesChange}
            onConnect={onConnect}
            onInit={setRfInstance}
            nodeTypes={nodeTypes}
            fitView
          >
            <Background />
            <Controls />
            <MiniMap />
          </ReactFlow>
        </div>

        <NodePalette />
      </div>
    </div>
  );
};

export default FlowEditor;
