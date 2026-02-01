import React, { useState, useEffect } from 'react';
import {
  GridComponent, ColumnsDirective, ColumnDirective,
  Page, Sort, Inject, DetailRow
} from '@syncfusion/ej2-react-grids';
import {
  BarChart, Bar, XAxis, YAxis, Tooltip, ResponsiveContainer
} from 'recharts';

import { Header } from '../components';

const API_KEY = "uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

// Dummy data - we'll change this later
const companiesData = [
  {
    ticker: 'AAPL',
    name: 'Apple Inc',
    pe: 28.5,
    pb: 47.2,
    price: 185.50,
    description: 'Apple Inc. designs, manufactures, and markets smartphones, personal computers, tablets, wearables, and accessories worldwide.',
    revenue: [
      { year: '2020', value: 274 },
      { year: '2021', value: 365 },
      { year: '2022', value: 394 },
      { year: '2023', value: 383 },
    ],
  },
  {
    ticker: 'MSFT',
    name: 'Microsoft Corporation',
    pe: 35.2,
    pb: 12.1,
    price: 420.30,
    description: 'Microsoft Corporation develops and licenses software, services, devices, and solutions worldwide.',
    revenue: [
      { year: '2020', value: 143 },
      { year: '2021', value: 168 },
      { year: '2022', value: 198 },
      { year: '2023', value: 211 },
    ],
  },
  {
    ticker: 'GOOGL',
    name: 'Alphabet Inc',
    pe: 25.8,
    pb: 6.2,
    price: 175.20,
    description: 'Alphabet Inc. offers various products and platforms in the United States, Europe, the Middle East, Africa, the Asia-Pacific, Canada, and Latin America.',
    revenue: [
      { year: '2020', value: 182 },
      { year: '2021', value: 257 },
      { year: '2022', value: 282 },
      { year: '2023', value: 307 },
    ],
  },
  {
    ticker: 'AMZN',
    name: 'Amazon.com Inc',
    pe: 62.4,
    pb: 8.5,
    price: 198.50,
    description: 'Amazon.com, Inc. engages in the retail sale of consumer products, advertising, and subscription services through online and physical stores worldwide.',
    revenue: [
      { year: '2020', value: 386 },
      { year: '2021', value: 469 },
      { year: '2022', value: 513 },
      { year: '2023', value: 574 },
    ],
  },
  {
    ticker: 'NVDA',
    name: 'NVIDIA Corporation',
    pe: 65.3,
    pb: 52.1,
    price: 875.40,
    description: 'NVIDIA Corporation provides graphics and compute and networking solutions in the United States, Taiwan, China, Hong Kong, and internationally.',
    revenue: [
      { year: '2020', value: 10 },
      { year: '2021', value: 16 },
      { year: '2022', value: 26 },
      { year: '2023', value: 60 },
    ],
  },
];

// Detail template - what shows when you expand a row
const detailTemplate = (props) => {
  return (
    <div className="p-4 bg-gray-50">

      {/* Top row: basic info */}
      <div className="flex gap-8 mb-4">
        <div><span className="font-bold">Ticker:</span> {props.ticker}</div>
        <div><span className="font-bold">Price:</span> ${props.price}</div>
      </div>

      {/* Description */}
      <p className="text-gray-600 mb-4">{props.description}</p>

      {/* Revenue Chart */}
      <div className="h-48 bg-white border rounded p-2">
        <p className="text-sm font-bold mb-2">Revenue (in billions $)</p>
        <ResponsiveContainer width="100%" height="85%">
          <BarChart data={props.revenue}>
            <XAxis dataKey="year" />
            <YAxis />
            <Tooltip />
            <Bar dataKey="value" fill="#03C9D7" />
          </BarChart>
        </ResponsiveContainer>
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
  const [companies, setCompanies] = useState(companiesData);
  const [loading, setLoading] = useState(true);

  // Fetch company logos on mount and merge into data
  useEffect(() => {
    const tickers = companiesData.map(c => c.ticker).join(',');

    fetch(`https://financialmodelingprep.com/api/v3/profile/${tickers}?apikey=${API_KEY}`)
      .then(res => res.json())
      .then(data => {
        console.log('API response:', data);
        if (Array.isArray(data)) {
          // Create a map of ticker -> logo
          const logoMap = {};
          data.forEach(company => {
            if (company.symbol && company.image) {
              logoMap[company.symbol] = company.image;
            }
          });
          // Merge logos into companies data
          const updatedCompanies = companiesData.map(c => ({
            ...c,
            logo: logoMap[c.ticker] || null
          }));
          setCompanies(updatedCompanies);
        }
      })
      .catch(err => console.error('Error fetching logos:', err))
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
        detailTemplate={detailTemplate}
        pageSettings={{ pageSize: 10 }}
        sortSettings={{ columns: [{ field: 'pe', direction: 'Ascending' }] }}
      >
        <ColumnsDirective>
          <ColumnDirective field="name" headerText="Company" width="250" template={companyTemplate} />
          <ColumnDirective field="pe" headerText="P/E" width="100" textAlign="Right" />
          <ColumnDirective field="pb" headerText="P/B" width="100" textAlign="Right" />
        </ColumnsDirective>

        <Inject services={[Page, Sort, DetailRow]} />

      </GridComponent>

    </div>
  );
};

export default Companies;
