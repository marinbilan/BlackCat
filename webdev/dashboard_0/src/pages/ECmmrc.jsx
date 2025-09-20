import React from 'react'
import { useEffect, useState } from "react";
import { BarChart, Bar, Line, Customized, ReferenceLine, XAxis, YAxis, Tooltip, Legend, ResponsiveContainer, Cell, PieChart } from "recharts";




import { ChevronDown, ChevronUp } from "lucide-react"; // icons

import { BsCurrencyDollar } from 'react-icons/bs';
import { FiSettings } from 'react-icons/fi';
import { GoPrimitiveDot } from 'react-icons/go';

import { Stacked, Pie, Button, Spark, SparkLine } from '../components';
import { earningData, SparklineAreaData, ecomPieChartDat } from '../data/dummy';
import { useStateContext } from '../contexts/ContextProvider';





function StockNews({ symbol }) {
  const [news, setNews] = useState([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const apiKey = "uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";
    const url = `https://financialmodelingprep.com/api/v3/stock_news?tickers=${symbol}&limit=5&apikey=${apiKey}`;

    fetch(url)
      .then(res => {
        if (!res.ok) throw new Error("Failed to fetch news");
        return res.json();
      })
      .then(data => {
        setNews(data);
      })
      .catch(err => {
        console.error("Error fetching news:", err);
      })
      .finally(() => {
        setLoading(false);
      });
  }, [symbol]);

  if (loading) return <p>Loading news...</p>;
  if (news.length === 0) return <p>No news found.</p>;

  return (
    <div className="space-y-4">
      {news.map((item, idx) => (
        <div key={idx} className="flex items-start space-x-3 p-2 border rounded-lg hover:bg-gray-50">
          {item.image && (
            <img
              src={item.image}
              alt={item.title}
              className="w-20 h-20 object-cover rounded-md"
            />
          )}
          <div className="flex-1">
            <a
              href={item.url}
              target="_blank"
              rel="noopener noreferrer"
              className="text-sm font-semibold text-blue-600 hover:underline"
            >
              {item.title}
            </a>
            <p className="text-xs text-gray-600 mt-1">{item.text}</p>
            <p className="text-xs text-gray-500 mt-1">{new Date(item.publishedDate).toLocaleString()}</p>
          </div>
        </div>
      ))}
    </div>
  );
}



function AnalystRatingBar({ symbol }) {
  const [segments, setSegments] = useState(null);
  const [total, setTotal] = useState(0);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  useEffect(() => {
    if (!symbol) return;
    setLoading(true);
    setError(null);
    setSegments(null);
    setTotal(0);

    fetch(
      `https://financialmodelingprep.com/api/v3/analyst-stock-recommendations/${encodeURIComponent(
        symbol
      )}?apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2`
    )
      .then((r) => {
        if (!r.ok) throw new Error(`HTTP ${r.status}`);
        return r.json();
      })
      .then((arr) => {
        if (!Array.isArray(arr) || arr.length === 0) {
          setError("No analyst data available.");
          return;
        }
       // use first (latest) element
        const raw = arr[0];

        // normalize keys to lowercase so we accept variants like
        // "analystRatingsbuy" or "analystRatingsBuy"
        const low = {};
        Object.keys(raw).forEach((k) => (low[k.toLowerCase()] = raw[k]));

        // parse counts defensively
        const strongBuy = Number(low["analystratingsstrongbuy"] ?? 0) || 0;
        const buy = Number(low["analystratingsbuy"] ?? 0) || 0;
        const hold = Number(low["analystratingshold"] ?? 0) || 0;
        const sell = Number(low["analystratingssell"] ?? 0) || 0;
        const strongSell = Number(low["analystratingsstrongsell"] ?? 0) || 0;

        const tot = strongBuy + buy + hold + sell + strongSell;
        if (tot === 0) {
          setError("No analyst ratings (all categories zero).");
          return;
        }

        const build = (name, count, color) => ({
          name,
          count,
          percent: (count / tot) * 100,
          color
        });

        // order segments as you requested (Strong Buy first)
        const data = [
          build("Strong Buy", strongBuy, "#065f46"), // dark green
          build("Buy", buy, "#10b981"),              // green
          build("Hold", hold, "#facc15"),            // yellow
          build("Sell", sell, "#ef4444"),            // red
          build("Strong Sell", strongSell, "#7f1d1d")// dark red
        ];
        setSegments(data);
        setTotal(tot);
      })
      .catch((err) => {
        console.error(err);
        setError("Failed to load analyst data.");
      })
      .finally(() => setLoading(false));
  }, [symbol]);

  if (loading) return <div className="text-sm text-gray-500">Loading ratings…</div>;
  if (error) return <div className="text-sm text-red-500">{error}</div>;
  if (!segments) return null;

  return (
    <div className="w-full max-w-xl">
      <div className="flex items-baseline justify-between mb-2">
        <div className="text-sm font-medium">Analyst Ratings ({total} analysts)</div>
        <div className="text-xs text-gray-500">{symbol}</div>
      </div>

      {/* horizontal segmented bar */}
      <div className="w-full h-6 bg-gray-200 rounded-lg overflow-hidden flex" role="img" aria-label="Analyst rating breakdown">
        {segments.map((s) => {
          // ensure a very small visible width for tiny segments
          const width = s.percent > 0 ? s.percent : 0;
          return (
            <div
              key={s.name}
              title={`${s.name}: ${s.count} (${s.percent.toFixed(1)}%)`}
              style={{ width: `${width}%`, backgroundColor: s.color }}
              className="flex items-center justify-center text-white text-xs"
            >
              {/* show percent label only if there's room */}
              {s.percent >= 6 ? `${s.percent.toFixed(0)}%` : null}
            </div>
          );
        })}
      </div>

      {/* legend with exact percentages */}
      <div className="mt-3 flex flex-wrap gap-3 text-xs">
        {segments.map((s) => (
          <div key={s.name} className="flex items-center space-x-2">
            <span className="w-3 h-3 rounded-sm" style={{ backgroundColor: s.color }} />
            <span>
              <strong>{s.name}</strong> • {s.count} ({s.percent.toFixed(1)}%)
            </span>
          </div>
        ))}
      </div>
    </div>
  );
}



const companies = [
  {
    ticker: "META",
    revAvg: 42.44,
    revLin: 63.13,
    revCAGR: 0.19,
    revenue: [
      { "2022_Q1": 11.47, "2022_Q2": 11.47, "2022_Q3": 11.03, "2022_Q4": 12.80, "2022": 22.22 },
      { "2023_Q1": 11.40, "2023_Q2": 12.73, "2023_Q3": 13.59, "2023_Q4": 15.96, "2023": 28.14 },
      { "2024_Q1": 14.51, "2024_Q2": 15.55, "2024_Q3": 16.15, "2024_Q4": 19.26, "2024": 34.21 }
    ],
    netIncome: [
      { year: 2019, Q1: 30, Q2: 35, Q3: 25, Q4: 40, Annual: 130 },
      { year: 2020, Q1: 32, Q2: 38, Q3: 30, Q4: 45, Annual: 145 }
    ]
  }
];



{/*{ label: `${row.year} Q1`, value: row.Q1, type: "quarter", year: row.year },*/}
function RevenueChart({ ticker, chartDataArg, annualAvg, annualLin, CAGR }) {

  const chartData = companies[0].revenue.flatMap(row => {
      // get the year key (numeric-looking)
      const yearKey = Object.keys(row).find(k => !k.includes("_"));
      
      // manually specify the order
      return [
        { label: `${yearKey}_Q1`, value: row[`${yearKey}_Q1`] },
        { label: `${yearKey}_Q2`, value: row[`${yearKey}_Q2`] },
        { label: `${yearKey}_Q3`, value: row[`${yearKey}_Q3`] },
        { label: `${yearKey}_Q4`, value: row[`${yearKey}_Q4`] },
        { label: yearKey, value: row[yearKey] } // annual last
      ];
    });

  return (
    <ResponsiveContainer width="30%" height={250}>
      <BarChart
        data={chartData}
        margin={{ top: 20, right: 20, left: 0, bottom: 10 }}
      >
        <XAxis
          dataKey="label"
          tick={{ fontSize: 10, fill: "#333" }}
          axisLine={false}
          tickLine={false}
        />
        <YAxis
          tick={{ fontSize: 12, fill: "#333" }}
          axisLine={false}
          tickLine={false}
        />
        <Tooltip
          cursor={{ fill: "rgba(0,0,0,0.05)" }}
          contentStyle={{ fontSize: 12 }}
        />
        {/* Custom text INSIDE plot */}
        <Customized
          component={({ width, height }) => (
            <text
              x={65}                 // margin.left + offset
              y={38}                // margin.top + offset
              fill="#333"
              fontSize={10}
            >
              <tspan fontWeight="bold">{ticker}</tspan>
              <tspan dx={8} fontWeight="bold">Annual Avg:</tspan> 
              <tspan dx={4}>{`$${annualAvg}`}</tspan>
              <tspan dx={8} fontWeight="bold">Annual Lin:</tspan> 
              <tspan dx={4}>{`$${annualLin}`}</tspan>
              <tspan dx={8}>{`CAGR: ${CAGR}`}</tspan>
            </text>
          )}
        />
        <Bar
          dataKey="value"
          name={`${ticker} Revenue`}
          radius={[3, 3, 0, 0]} // rounded top corners
          barSize={11}          // adjust width
        >
          {chartData.map((entry, index) => (
            <Cell
              key={`cell-${index}`}
              fill={entry.label.includes("Annual") || entry.label.length === 4 ? "#065f46" : "#facc15"}
            />
          ))}
        </Bar>

       <ReferenceLine y={annualAvg} stroke="green" strokeWidth={1} /> 

      </BarChart>
    </ResponsiveContainer>
  );        
}




function FinancialTabs({ company }) {
  const [activeTab, setActiveTab] = useState("netIncome");

  const tabs = [
    { key: "revenue", label: "Revenue" },
    { key: "netIncome", label: "Net Income" }
  ];

  const getChartData = () => {
    if (activeTab === "revenue") return company.revenue;
    if (activeTab === "netIncome") return company.netIncome;
    return [];
  };

  const getMetrics = () => {
    if (activeTab === "revenue")
      return { annualAvg: company.revAvg, annualLin: company.revLin, CAGR: company.revCAGR };
    if (activeTab === "netIncome")
      return { annualAvg: company.netIncomeAvg, annualLin: company.netIncomeLin, CAGR: company.netIncomeCAGR };
    return {};
  };

  const chartData = getChartData();
  const metrics = getMetrics();

  return (
    <div>
      {/* Tabs */}
      <div className="flex space-x-2 mb-4">
        {tabs.map(tab => (
          <button
            key={tab.key}
            className={`px-3 py-1 rounded ${
              activeTab === tab.key ? "bg-blue-500 text-white" : "bg-gray-200"
            }`}
            onClick={() => setActiveTab(tab.key)}
          >
            {tab.label}
          </button>
        ))}
      </div>

      {/* Chart */}
      <RevenueChart
        ticker={company.ticker}
        chartDataArg={chartData}
        annualAvg={metrics.annualAvg}
        annualLin={metrics.annualLin}
        CAGR={metrics.CAGR}
      />
    </div>
  );
}










const ECmmrc = () => {
  const [profile, setProfile] = useState(null);
  const [expanded, setExpanded] = useState(false);


  useEffect(() => {
    fetch("https://financialmodelingprep.com/stable/profile?symbol=AAPL&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2")
      .then((res) => res.json())
      .then((data) => setProfile(data[0])) // API returns array
      .catch((err) => console.error(err));
  }, []);

  if (!profile) return <p className="text-gray-500">Loading...</p>;


  return (
    <div className="items-center p-8 gap-6">

      <div className="flex items-center p-8 gap-6">
        {/* Rounded picture */}
        <div className="w-12 h-12 rounded-full overflow-hidden shadow-md">
          <img
            src={profile.image}
            alt={profile.companyName}
            className="w-full h-full object-contain bg-white"
          />
        </div>
        <div>
          <h2 className="text-lg font-semibold">{profile.companyName}</h2>
          <p className="text-xs text-gray-600">{profile.symbol} · {profile.industry}</p>
          <p className="text-xs text-gray-600">
            Price: ${profile.price}
        <p className={`text-sm font-semibold ${
          profile.change >= 0 ? "text-green-600" : "text-red-600"
          }`}>
          {profile.change >= 0 ? "▲" : "▼"} ${profile.change.toFixed(2)} ({profile.changePercentage.toFixed(2)}%)
        </p>

          </p>
        </div>


        <div className="">
        {/* Example text */}
          <h2 className="text-lg font-semibold">Sector: {profile.sector}</h2>
          <p className="text-xs text-gray-600">{profile.symbol} · {profile.industry}</p>
        </div>

        <p className={`text-sm font-semibold ${
          profile.change >= 0 ? "text-green-600" : "text-red-600"
          }`}>
          {profile.change >= 0 ? "▲" : "▼"} ${profile.change.toFixed(2)} ({profile.changePercentage.toFixed(2)}%)
        </p>
      </div>

      <div className="w-full border rounded-lg p-2 shadow-sm bg-white">
        {/* Header with toggle button */}
        <div className="flex justify-between items-center cursor-pointer" 
             onClick={() => setExpanded(!expanded)}>
          <h3 className="text-sm font-semibold text-gray-800">Summary</h3>
          {expanded ? (
            <ChevronUp className="w-4 h-4 text-gray-500" />
          ) : (
            <ChevronDown className="w-4 h-4 text-gray-500" />
          )}
        </div>

        {/* Expandable content */}
        {expanded && (
          <div className="mt-2">
            <p className="text-xs text-gray-600">{profile.description}</p>
          </div>
        )}
      </div>

      {/* NEWS SECTION NEWS SECTION NEWS SECTION*/}
      {/* Call the StockNews component */}
      <div>
        <StockNews symbol={profile.symbol} />
      </div>

      <div>
        <AnalystRatingBar symbol={profile.symbol} />
      </div>

      <div>
        <RevenueChart 
          ticker={companies[0].ticker} 
          chartDataArg={companies[0].revenue} 
          annualAvg={companies[0].revAvg}
          annualLin={companies[0].revLin}
          CAGR={companies[0].revCAGR} 
        />
      </div>
      <div>
        <RevenueChart 
          ticker={companies[0].ticker} 
          chartDataArg={companies[0].revenue} 
          annualAvg={companies[0].revAvg}
          annualLin={companies[0].revLin}
          CAGR={companies[0].revCAGR} 
        />
      </div>


      <div>
        < FinancialTabs company={companies[0]} />
      </div>
    </div>

  )
}

export default ECmmrc