import React, { useState, useEffect } from 'react';
import {
  GridComponent, ColumnsDirective, ColumnDirective,
  Page, Sort, Inject, DetailRow
} from '@syncfusion/ej2-react-grids';
import {
  ComposedChart, Bar, Line, XAxis, YAxis, Tooltip, ResponsiveContainer,
  Legend, Cell
} from 'recharts';

import { Header } from '../components';

const API_KEY = "uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

// Cache for company descriptions to avoid repeat API calls
const descriptionCache = {};

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
      <p className="text-gray-600 mb-4 text-sm">
        {loadingDesc ? 'Loading description...' : (description || 'No description available')}
      </p>

      {/* Top row: basic info */}
      <div className="flex gap-8 mb-4">
        <div><span className="font-bold">Ticker:</span> {data.ticker}</div>
        <div><span className="font-bold">Price:</span> ${data.price?.toFixed(2)}</div>
        <div><span className="font-bold">Market Cap:</span> ${(data.marketCap / 1e9).toFixed(1)}B</div>
        <div><span className="font-bold">Shares Outstanding:</span> {(data.sharesOutstanding / 1e9).toFixed(2)}B</div>
      </div>

      {/* Key metrics with marks */}
      <div className="grid grid-cols-4 gap-x-8 gap-y-1 mb-4 text-sm bg-white border rounded p-3">
        <div><span className="font-bold">P/E:</span> {data.pe?.toFixed(1)} {data.peCalculated !== undefined && <>(calc: {data.peCalculated?.toFixed(1)})</>} {data.peMark !== undefined && <span className="text-blue-600">[{data.peMark}]</span>}</div>
        <div><span className="font-bold">P/B:</span> {data.pb?.toFixed(2)} {data.pbCalculated !== undefined && <>(calc: {data.pbCalculated?.toFixed(2)})</>} {data.pbMark !== undefined && <span className="text-blue-600">[{data.pbMark}]</span>}</div>
        <div><span className="font-bold">Net Margin:</span> {data.netMargin?.toFixed(1)}% {data.netMarginMark !== undefined && <span className="text-blue-600">[{data.netMarginMark}]</span>}</div>
        <div><span className="font-bold">RoE:</span> {data.roe?.toFixed(1)}% {data.roeMark !== undefined && <span className="text-blue-600">[{data.roeMark}]</span>}</div>
        <div><span className="font-bold">D/E:</span> {(data.debtToEquity ?? data.de)?.toFixed(2)} {data.debtToEquityMark !== undefined && <span className="text-blue-600">[{data.debtToEquityMark}]</span>}</div>
        <div><span className="font-bold">YrsToRetDebt:</span> {(data.yrsToRetDebt ?? data.yrsToRetDbt)?.toFixed(1)} {data.yrsToRetDebtMark !== undefined && <span className="text-blue-600">[{data.yrsToRetDebtMark}]</span>}</div>
        <div><span className="font-bold">Dividend Yield:</span> {data.dividendYield?.toFixed(2) ?? '-'}%</div>
        <div><span className="font-bold">Company CAGR:</span> {((data.companyAvgCAGR ?? data.companyGrowth) * 100)?.toFixed(1)}%</div>
        <div className="col-span-4 border-t pt-2 mt-2">
          <span className="font-bold text-lg">Total Mark: {data.totalMark}</span>
        </div>
      </div>

      {/* DCF Prices */}
      <div className="grid grid-cols-3 gap-4 mb-4 text-sm bg-white border rounded p-3">
        <div><span className="font-bold">Zero Growth Price:</span> ${(data.zeroGrowthPrice ?? data.dcfZeroGr)?.toFixed(2)}</div>
        <div><span className="font-bold">DCF Growth Price:</span> ${data.dcfGrowthPrice?.toFixed(2) ?? '-'}</div>
        <div><span className="font-bold">Graham PE Growth:</span> ${(data.grahamPeGrPrice ?? data.grahamPeGr)?.toFixed(2)}</div>
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
