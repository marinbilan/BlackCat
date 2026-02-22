import React, { useState, useEffect } from 'react';
import {
  GridComponent, ColumnsDirective, ColumnDirective,
  Page, Sort, Inject, DetailRow
} from '@syncfusion/ej2-react-grids';
import {
  ComposedChart, Bar, Line, XAxis, YAxis, Tooltip, ResponsiveContainer,
  Legend, Cell, AreaChart, Area, ReferenceLine
} from 'recharts';

import { Header } from '../components';

const API_KEY = "uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

// Star rating component for marks
const StarRating = ({ value, max = 10 }) => {
  if (value === undefined || value === null) return null;

  const filled = Math.min(Math.round(value), max);
  const empty = max - filled;

  // Color thresholds based on max stars
  // max 10: 1-3 red, 4-7 yellow, 8-10 green
  // max 8:  1-3 red, 4-6 yellow, 7-8 green
  // max 5:  1 red, 2-4 yellow, 5 green
  let colorClass = 'text-red-500';
  if (max === 10) {
    if (filled >= 8) colorClass = 'text-green-500';
    else if (filled >= 4) colorClass = 'text-yellow-500';
  } else if (max === 8) {
    if (filled >= 7) colorClass = 'text-green-500';
    else if (filled >= 4) colorClass = 'text-yellow-500';
  } else if (max === 5) {
    if (filled >= 5) colorClass = 'text-green-500';
    else if (filled >= 2) colorClass = 'text-yellow-500';
  }

  return (
    <span className={`inline-flex gap-px ${colorClass}`} title={`${value}/${max}`}>
      {[...Array(filled)].map((_, i) => (
        <span key={`f${i}`}>&#9733;</span>
      ))}
      {[...Array(empty)].map((_, i) => (
        <span key={`e${i}`} className="opacity-30">&#9733;</span>
      ))}
    </span>
  );
};

// Cache for company descriptions to avoid repeat API calls
const descriptionCache = {};

// Cache for price data
const priceDataCache = {};

// Stock price chart component
const StockPriceChart = ({ ticker, zeroGrPrice, dcfGrPrice }) => {
  const [priceData, setPriceData] = useState([]);
  const [loading, setLoading] = useState(true);
  const [period, setPeriod] = useState('MAX');

  const periods = [
    { label: '1M', days: 30 },
    { label: '3M', days: 90 },
    { label: '6M', days: 180 },
    { label: '1Y', days: 365 },
    { label: '5Y', days: 1825 },
    { label: 'MAX', days: 9999 },
  ];

  useEffect(() => {
    const cacheKey = `${ticker}_${period}`;
    if (priceDataCache[cacheKey]) {
      setPriceData(priceDataCache[cacheKey]);
      setLoading(false);
      return;
    }

    setLoading(true);
    const selectedPeriod = periods.find(p => p.label === period);
    const days = selectedPeriod?.days || 365;

    fetch(`https://financialmodelingprep.com/api/v3/historical-price-full/${ticker}?apikey=${API_KEY}`)
      .then(res => res.json())
      .then(data => {
        if (data?.historical) {
          // Filter by period and reverse for chronological order
          const filtered = data.historical
            .slice(0, days)
            .reverse()
            .map(d => ({
              date: d.date,
              price: d.close,
            }));
          priceDataCache[cacheKey] = filtered;
          setPriceData(filtered);
        }
      })
      .catch(err => console.error('Error fetching price data:', err))
      .finally(() => setLoading(false));
  }, [ticker, period]);

  // Calculate price change
  const priceChange = priceData.length >= 2
    ? ((priceData[priceData.length - 1].price - priceData[0].price) / priceData[0].price * 100)
    : 0;
  const isPositive = priceChange >= 0;

  return (
    <div className="bg-white border rounded p-3 font-['Inter',sans-serif] h-full">
      <div className="flex items-center justify-between mb-2">
        <div className="flex items-center gap-2">
          <span className="text-sm font-semibold text-gray-700">Price Chart</span>
          {!loading && priceData.length > 0 && (
            <span className={`text-xs font-medium ${isPositive ? 'text-green-600' : 'text-red-600'}`}>
              {isPositive ? '+' : ''}{priceChange.toFixed(2)}%
            </span>
          )}
        </div>
        <div className="flex gap-1">
          {periods.map(p => (
            <button
              key={p.label}
              onClick={() => setPeriod(p.label)}
              className={`px-2 py-0.5 text-xs rounded transition-colors ${
                period === p.label
                  ? 'bg-blue-500 text-white'
                  : 'bg-gray-100 text-gray-600 hover:bg-gray-200'
              }`}
            >
              {p.label}
            </button>
          ))}
        </div>
      </div>

      {loading ? (
        <div className="h-48 flex items-center justify-center text-gray-400 text-sm">Loading...</div>
      ) : (
        <ResponsiveContainer width="100%" height={192}>
          <AreaChart data={priceData}>
            <defs>
              <linearGradient id={`gradient_${ticker}`} x1="0" y1="0" x2="0" y2="1">
                <stop offset="5%" stopColor={isPositive ? '#22c55e' : '#ef4444'} stopOpacity={0.3}/>
                <stop offset="95%" stopColor={isPositive ? '#22c55e' : '#ef4444'} stopOpacity={0}/>
              </linearGradient>
            </defs>
            <XAxis
              dataKey="date"
              tick={{ fontSize: 9 }}
              tickFormatter={(val) => {
                const d = new Date(val);
                return period === '1M' || period === '3M'
                  ? `${d.getDate()}/${d.getMonth() + 1}`
                  : `${d.getMonth() + 1}/${d.getFullYear().toString().slice(2)}`;
              }}
              interval="preserveStartEnd"
              minTickGap={30}
            />
            <YAxis
              tick={{ fontSize: 9 }}
              domain={['auto', 'auto']}
              tickFormatter={(val) => `$${val.toFixed(0)}`}
              width={45}
            />
            <Tooltip
              contentStyle={{ fontSize: 11 }}
              formatter={(value) => [`$${value.toFixed(2)}`, 'Price']}
              labelFormatter={(label) => new Date(label).toLocaleDateString()}
            />
            <Area
              type="monotone"
              dataKey="price"
              stroke={isPositive ? '#22c55e' : '#ef4444'}
              strokeWidth={1.5}
              fill={`url(#gradient_${ticker})`}
            />
            {zeroGrPrice && (
              <ReferenceLine
                y={zeroGrPrice}
                stroke="#22c55e"
                strokeWidth={1.5}
                label={{ value: `ZeroGr $${zeroGrPrice.toFixed(0)}`, fontSize: 9, fill: '#22c55e', position: 'right' }}
              />
            )}
            {dcfGrPrice && (
              <ReferenceLine
                y={dcfGrPrice}
                stroke="#eab308"
                strokeWidth={1.5}
                label={{ value: `DCFGr $${dcfGrPrice.toFixed(0)}`, fontSize: 9, fill: '#eab308', position: 'right' }}
              />
            )}
          </AreaChart>
        </ResponsiveContainer>
      )}
    </div>
  );
};

// Detail template component - fetches description on mount
const CompanyDetail = ({ data }) => {
  const [description, setDescription] = useState(null);
  const [loadingDesc, setLoadingDesc] = useState(true);

  useEffect(() => {
    // Check cache first
    if (descriptionCache[data.ticker]) {
      setDescription(descriptionCache[data.ticker]);
      setLoadingDesc(false);
      return;
    }

    fetch(`https://financialmodelingprep.com/stable/profile?symbol=${data.ticker}&apikey=${API_KEY}`)
      .then(res => res.json())
      .then(profile => {
        if (Array.isArray(profile) && profile[0]) {
          const desc = profile[0].description;
          descriptionCache[data.ticker] = desc;
          setDescription(desc);
        }
      })
      .catch(err => console.error('Error fetching description:', err))
      .finally(() => setLoadingDesc(false));
  }, [data.ticker]);

  // Filter data by type
  const financialsYearly = data.financials?.filter(f => f.type === 'annual') || [];
  const financialsQuarterly = data.financials?.filter(f => f.type === 'quarter') || [];
  const balanceSheetYearly = data.balanceSheet?.filter(b => b.type === 'annual') || [];
  const balanceSheetQuarterly = data.balanceSheet?.filter(b => b.type === 'quarter') || [];

  return (
    <div className="p-4 bg-gray-50">

      {/* Company description */}
      <p className="text-gray-600 mb-4 text-sm font-['Inter',sans-serif]">
        {loadingDesc ? 'Loading description...' : (description || 'No description available')}
      </p>

      {/* Key metrics and Price Chart side by side */}
      <div className="flex gap-4 mb-4">
        {/* Key metrics with marks */}
        <div className="w-[calc(50%-0.5rem)] flex flex-col text-xs bg-white border rounded p-3 font-['Inter',sans-serif]">
          <div className="flex gap-2">
            {/* Left column - Stars */}
            <div className="flex flex-col gap-1">
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-28">Market Cap</span> <span className="text-gray-600 w-40">${(data.marketCap / 1e9).toFixed(1)}B</span></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-28">Shares Out.</span> <span className="text-gray-600 w-40">{(data.sharesOutstanding / 1e9).toFixed(2)}B</span></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-28">P/E</span> <span className="text-gray-600 w-40">{data.pe?.toFixed(1)} {data.peCalculated !== undefined && <>(calc: {data.peCalculated?.toFixed(1)})</>}</span> <StarRating value={data.peMark} /></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-28">P/B</span> <span className="text-gray-600 w-40">{data.pb?.toFixed(2)} {data.pbCalculated !== undefined && <>(calc: {data.pbCalculated?.toFixed(2)})</>}</span> <StarRating value={data.pbMark} /></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-28">Net Margin</span> <span className="text-gray-600 w-40">{(data.netMargin * 100)?.toFixed(1)}%</span> <StarRating value={data.netMarginMark} max={8} /></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-28">RoE</span> <span className="text-gray-600 w-40">{(data.roe * 100)?.toFixed(1)}%</span> <StarRating value={data.roeMark} max={8} /></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-28">D/E</span> <span className="text-gray-600 w-40">{(data.debtToEquity ?? data.de)?.toFixed(2)}</span> <StarRating value={data.debtToEquityMark} max={5} /></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-28">YrsToRetDebt</span> <span className="text-gray-600 w-40">{(data.yrsToRetDebt ?? data.yrsToRetDbt)?.toFixed(1)}</span> <StarRating value={data.yrsToRetDebtMark} /></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-28">Dividend Yield</span> <span className="text-gray-600 w-40">{data.dividendYield?.toFixed(2) ?? '-'}%</span></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-28">Company CAGR</span> <span className="text-gray-600 w-40">{((data.companyAvgCAGR ?? data.companyGrowth) * 100)?.toFixed(1)}%</span></div>
            </div>
            {/* Right column - Price & Averages */}
            <div className="flex flex-col gap-1 border-l pl-2">
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-20">Price</span> <span className="text-gray-600">${data.price?.toFixed(2)}</span></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-20">ZeroGr</span> <span className="text-gray-600">${data.zeroGrowthPrice?.toFixed(2)}</span></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-20">DCFGr</span> <span className="text-gray-600">${data.dcfGrowthPrice?.toFixed(2)}</span></div>
              <div className="h-2"></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-20">Avg NetInc</span> <span className="text-gray-600">${data.netIncAvg?.toFixed(2)}</span></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-20">Avg FCF</span> <span className="text-gray-600">${data.fcfAvg?.toFixed(2)}</span></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-20">Avg Cash</span> <span className="text-gray-600">${data.cashAvg?.toFixed(2)}</span></div>
              <div className="flex items-center"><span className="font-semibold text-gray-700 w-20">Avg Debt</span> <span className="text-gray-600">${data.totDebtAvg?.toFixed(2)}</span></div>
            </div>
          </div>
          <div className="border-t pt-2 mt-1">
            <span className="font-semibold text-gray-800">Total Mark: {data.totalMark}</span>
          </div>
        </div>

        {/* Stock Price Chart */}
        <div className="w-[calc(50%-0.5rem)]">
          <StockPriceChart
            ticker={data.ticker}
            zeroGrPrice={data.zeroGrowthPrice ?? data.dcfZeroGr}
            dcfGrPrice={data.dcfGrowthPrice}
          />
        </div>
      </div>

      {/* Financials Charts - Yearly (left) and Quarterly (right) */}
      <div className="flex gap-4 mb-4">
        {/* Yearly Financials */}
        <div className="flex-1 h-72 bg-white border rounded p-2">
          <p className="text-sm font-bold mb-1">Financials - Yearly (per share $)</p>
          {data.revAvg !== undefined && (
            <p className="text-xs text-gray-500 mb-1">
              Avg: Rev ${data.revAvg?.toFixed(2)} | NetInc ${data.netIncAvg?.toFixed(2)} | Margin {(data.netIncRatioAvg * 100)?.toFixed(1)}% | FCF ${data.fcfAvg?.toFixed(2)}
            </p>
          )}
          <ResponsiveContainer width="100%" height="85%">
            <ComposedChart data={financialsYearly} barGap={0} barCategoryGap="15%">
              <XAxis
                dataKey="period"
                tick={{ fontSize: 10 }}
                interval={0}
                angle={-45}
                textAnchor="end"
                height={40}
              />
              <YAxis yAxisId="left" tick={{ fontSize: 10 }} />
              <YAxis
                yAxisId="right"
                orientation="right"
                tick={{ fontSize: 10 }}
                tickFormatter={(v) => `${(v * 100).toFixed(0)}%`}
                domain={[0, 0.6]}
              />
              <Tooltip
                formatter={(value, name) => {
                  if (name === 'Net Income Ratio') return `${(value * 100).toFixed(1)}%`;
                  return `$${value.toFixed(2)}`;
                }}
              />
              <Legend />
              <Bar dataKey="revenue" name="Revenue" yAxisId="left" fill="#B91C1C" />
              <Bar dataKey="netIncome" name="Net Income" yAxisId="left" fill="#1D4ED8" />
              <Bar dataKey="fcf" name="FCF" yAxisId="left" fill="#1B7A2B" />
              <Line
                dataKey="netIncomeRatio"
                name="Net Income Ratio"
                yAxisId="right"
                type="monotone"
                stroke="#F59E0B"
                strokeWidth={2}
                dot={false}
              />
            </ComposedChart>
          </ResponsiveContainer>
        </div>

        {/* Quarterly Financials */}
        <div className="flex-1 h-72 bg-white border rounded p-2">
          <p className="text-sm font-bold mb-1">Financials - Quarterly (per share $)</p>
          <ResponsiveContainer width="100%" height="90%">
            <ComposedChart data={financialsQuarterly} barGap={0} barCategoryGap="15%">
              <XAxis
                dataKey="period"
                tick={{ fontSize: 10 }}
                interval={0}
                angle={-45}
                textAnchor="end"
                height={40}
              />
              <YAxis yAxisId="left" tick={{ fontSize: 10 }} />
              <YAxis
                yAxisId="right"
                orientation="right"
                tick={{ fontSize: 10 }}
                tickFormatter={(v) => `${(v * 100).toFixed(0)}%`}
                domain={[0, 0.6]}
              />
              <Tooltip
                formatter={(value, name) => {
                  if (name === 'Net Income Ratio') return `${(value * 100).toFixed(1)}%`;
                  return `$${value.toFixed(2)}`;
                }}
              />
              <Legend />
              <Bar dataKey="revenue" name="Revenue" yAxisId="left" fill="#EF4444" />
              <Bar dataKey="netIncome" name="Net Income" yAxisId="left" fill="#60A5FA" />
              <Bar dataKey="fcf" name="FCF" yAxisId="left" fill="#4ADE80" />
              <Line
                dataKey="netIncomeRatio"
                name="Net Income Ratio"
                yAxisId="right"
                type="monotone"
                stroke="#F59E0B"
                strokeWidth={2}
                dot={false}
              />
            </ComposedChart>
          </ResponsiveContainer>
        </div>
      </div>

      {/* Balance Sheet Charts - Yearly (left) and Quarterly (right) */}
      <div className="flex gap-4">
        {/* Yearly Balance Sheet */}
        <div className="flex-1 h-72 bg-white border rounded p-2">
          <p className="text-sm font-bold mb-1">Balance Sheet - Yearly (per share $)</p>
          {data.cashAvg !== undefined && (
            <p className="text-xs text-gray-500 mb-1">
              Avg: Cash ${data.cashAvg?.toFixed(2)} | Equity ${data.shEqAvg?.toFixed(2)} | Debt ${data.totDebtAvg?.toFixed(2)}
            </p>
          )}
          <ResponsiveContainer width="100%" height="85%">
            <ComposedChart data={balanceSheetYearly} barGap={0} barCategoryGap="15%">
              <XAxis
                dataKey="period"
                tick={{ fontSize: 10 }}
                interval={0}
                angle={-45}
                textAnchor="end"
                height={40}
              />
              <YAxis tick={{ fontSize: 10 }} />
              <Tooltip formatter={(value) => `$${value.toFixed(2)}`} />
              <Legend />
              <Bar dataKey="cash" name="Cash" fill="#1B7A2B" />
              <Bar dataKey="totalDebt" name="Total Debt" fill="#B91C1C" />
              <Bar dataKey="shareholderEquity" name="Equity" fill="#1D4ED8" />
            </ComposedChart>
          </ResponsiveContainer>
        </div>

        {/* Quarterly Balance Sheet */}
        <div className="flex-1 h-72 bg-white border rounded p-2">
          <p className="text-sm font-bold mb-1">Balance Sheet - Quarterly (per share $)</p>
          <ResponsiveContainer width="100%" height="90%">
            <ComposedChart data={balanceSheetQuarterly} barGap={0} barCategoryGap="15%">
              <XAxis
                dataKey="period"
                tick={{ fontSize: 10 }}
                interval={0}
                angle={-45}
                textAnchor="end"
                height={40}
              />
              <YAxis tick={{ fontSize: 10 }} />
              <Tooltip formatter={(value) => `$${value.toFixed(2)}`} />
              <Legend />
              <Bar dataKey="cash" name="Cash" fill="#4ADE80" />
              <Bar dataKey="totalDebt" name="Total Debt" fill="#EF4444" />
              <Bar dataKey="shareholderEquity" name="Equity" fill="#60A5FA" />
            </ComposedChart>
          </ResponsiveContainer>
        </div>
      </div>

    </div>
  );
};

// Template for company column with logo
const companyTemplate = (props) => (
  <div className="flex items-center gap-3">
    {props.logo ? (
      <img
        src={props.logo}
        alt={props.ticker}
        className="w-8 h-8 rounded-full object-contain bg-white"
      />
    ) : (
      <div className="w-8 h-8 rounded-full bg-gray-200 flex items-center justify-center text-xs">
        {props.ticker?.charAt(0)}
      </div>
    )}
    <span>{props.name}</span>
  </div>
);

const Companies = () => {
  const [companies, setCompanies] = useState([]);
  const [loading, setLoading] = useState(true);

  // Load companies from JSON file and fetch logos
  useEffect(() => {
    fetch('/companiesData.json')
      .then(res => {
        if (!res.ok) {
          throw new Error('Failed to load companies data');
        }
        return res.json();
      })
      .then(data => {
        if (!Array.isArray(data) || data.length === 0) {
          console.warn('No companies data found in JSON file');
          setCompanies([]);
          setLoading(false);
          return;
        }

        // Fetch logos for each company
        const tickers = data.map(c => c.ticker).join(',');
        return fetch(`https://financialmodelingprep.com/api/v3/profile/${tickers}?apikey=${API_KEY}`)
          .then(res => res.json())
          .then(logos => {
            const logoMap = {};
            if (Array.isArray(logos)) {
              logos.forEach(company => {
                if (company.symbol && company.image) {
                  logoMap[company.symbol] = company.image;
                }
              });
            }
            return data.map(c => ({ ...c, logo: logoMap[c.ticker] || null }));
          })
          .catch(logoErr => {
            console.error('Error fetching logos:', logoErr);
            // Return companies without logos if logo fetch fails
            return data.map(c => ({ ...c, logo: null }));
          });
      })
      .then(companiesWithLogos => {
        if (companiesWithLogos) {
          // Normalize field names for backward compatibility
          const normalized = companiesWithLogos.map(c => ({
            ...c,
            debtToEquity: c.debtToEquity ?? c.de,
            yrsToRetDebt: c.yrsToRetDebt ?? c.yrsToRetDbt,
          }));
          setCompanies(normalized);
        }
      })
      .catch(err => {
        console.error('Error loading companies:', err);
        setCompanies([]);
      })
      .finally(() => setLoading(false));
  }, []);

  if (loading) {
    return (
      <div className="m-2 md:m-10 p-2 md:p-10 bg-white rounded-3xl">
        <Header category="Page" title="Companies" />
        <p className="text-gray-500">Loading...</p>
      </div>
    );
  }

  return (
    <div className="m-2 md:m-10 p-2 md:p-10 bg-white rounded-3xl">
      <Header category="Page" title="Companies" />

      <GridComponent
        dataSource={companies}
        allowPaging
        allowSorting
        allowMultiSorting
        detailTemplate={(props) => <CompanyDetail data={props} />}
        pageSettings={{ pageSize: 10 }}
        sortSettings={{ columns: [{ field: 'totalMark', direction: 'Descending' }] }}
      >
        <ColumnsDirective>
          <ColumnDirective field="name" headerText="Company" width="180" template={companyTemplate} />
          <ColumnDirective field="ticker" headerText="Ticker" width="70" />
          <ColumnDirective field="totalMark" headerText="Mark" width="80" textAlign="Right" />
          <ColumnDirective field="price" headerText="Price" width="80" textAlign="Right" format="C2" />
          <ColumnDirective field="zeroGrowthPrice" headerText="ZeroGr" width="80" textAlign="Right" format="C2" />
          <ColumnDirective field="dcfGrowthPrice" headerText="DCFGr" width="80" textAlign="Right" format="C2" />
          <ColumnDirective field="grahamPeGrPrice" headerText="GrahamPE" width="90" textAlign="Right" format="C2" />
          <ColumnDirective field="pe" headerText="P/E" width="60" textAlign="Right" format="N2" />
          <ColumnDirective field="pb" headerText="P/B" width="60" textAlign="Right" format="N2" />
          <ColumnDirective field="netMargin" headerText="NetMarg" width="80" textAlign="Right" format="N4" />
          <ColumnDirective field="roe" headerText="RoE" width="70" textAlign="Right" format="N4" />
          <ColumnDirective field="yrsToRetDebt" headerText="YrRetDbt" width="80" textAlign="Right" format="N1" />
        </ColumnsDirective>

        <Inject services={[Page, Sort, DetailRow]} />

      </GridComponent>

    </div>
  );
};

export default Companies;
