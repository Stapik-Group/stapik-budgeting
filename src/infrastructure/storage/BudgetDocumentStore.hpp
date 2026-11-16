#pragma once

#include "../../core/model/BudgetDocument.hpp"

#include "stapik/document/DocumentFile.hpp"

class BudgetDocumentStore
{
public:
    BudgetDocumentStore();

    [[nodiscard]] BudgetDocument load() const;
    void save(const BudgetDocument& document) const;

private:
    stapik::document::DocumentFile<BudgetDocument> m_file;

    void moveUnusableFileAside() const;
};
