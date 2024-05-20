import re

print("Test Test")

text = """
≡ CRM - Salesforce.com 3.22 25,815 $301.18 $7,775,000 $287.08 -4.68% $193.43 $318.30
≡ JWN - Nordstrom 3.17 377,226 $20.27 $7,646,000 $21.10 4.09% $12.58 $22.69
≡ BK - Bank of New York Mellon Corp. 3.10 130,000 $57.62 $7,491,000 $58.97 2.34% $38.55 $59.70
≡ EMR - Emerson Electric 3.10 66,000 $113.42 $7,486,000 $113.71 0.26% $75.35 $116.23
≡ GSK - GSK plc 3.07 172,865 $42.87 $7,411,000 $44.60 4.04% $32.08 $45.55
≡ AMZN - Amazon.com Inc. 3.06 41,000 $180.39 $7,396,000 $183.62 1.79% $113.78 $191.70
≡ TSM - Taiwan Semiconductor S.A. 2.97 52,729 $136.05 $7,174,000 $153.62 12.91% $83.28 $157.76
≡ DD - DuPont de Nemours Inc. 2.95 Add 22.24% 92,900 $76.67 $7,123,000 $79.90 4.21% $60.81 $80.41
≡ WFC - Wells Fargo 2.95 122,655 $57.96 $7,109,000 $60.72 4.76% $37.56 $62.55
≡ USB - U.S. Bancorp 2.89 156,000 $44.70 $6,973,000 $40.91 -8.48% $27.83 $44.91
"""

tickers = re.findall(r'(?<=≡\s)([A-Z]+)\s-', text)

cpp_vector_declaration = "std::vector<std::string> tickers = {\n"

for ticker in tickers:
    cpp_vector_declaration += f'    "{ticker}",\n'

cpp_vector_declaration += "};\n"

print(cpp_vector_declaration)
