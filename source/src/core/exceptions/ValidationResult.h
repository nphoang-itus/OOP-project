#ifndef VALIDATION_RESULT_H
#define VALIDATION_RESULT_H

#include "Result.h"
#include <vector>
#include <string>

struct ValidationError
{
    std::string fieldName;
    std::string message;
    std::string errorCode;

    ValidationError(std::string fieldName, std::string message, std::string errorCode = "")
        : fieldName(std::move(fieldName)), message(std::move(message)), errorCode(std::move(errorCode)) {}
};

class ValidationResult
{
private:
    std::vector<ValidationError> _errors;

public:
    ValidationResult() = default;

    void addError(ValidationError error)
    {
        _errors.push_back(std::move(error));
    }

    void addError(const std::string &fieldName, const std::string &message, const std::string &errorCode = "")
    {
        _errors.emplace_back(fieldName, message, errorCode);
    }

    bool isValid() const
    {
        return _errors.empty();
    }

    const std::vector<ValidationError> &getErrors() const
    {
        return _errors;
    }

    std::string getErrorMessages() const
    {
        if (_errors.empty())
            return "";

        std::string result;
        for (size_t i = 0; i < _errors.size(); ++i)
        {
            if (i > 0)
                result += "; ";
            result += _errors[i].fieldName + ": " + _errors[i].message;
        }
        return result;
    }

    void combine(const ValidationResult &other)
    {
        _errors.insert(_errors.end(), other._errors.begin(), other._errors.end());
    }
};

inline VoidResult toResult(const ValidationResult &validation)
{
    if (validation.isValid())
    {
        return Success();
    }
    return Failure(CoreError(validation.getErrorMessages(), "VALIDATION_ERROR"));
}

template <typename T>
Result<T> getValidationFailure(const ValidationResult &validationResult)
{
    std::string typeErrors;
    for (size_t i = 0; i < validationResult.getErrors().size(); ++i)
    {
        if (i > 0)
            typeErrors += ';';
        typeErrors += validationResult.getErrors()[i].errorCode;
    }
    return std::unexpected(CoreError(
        validationResult.getErrorMessages(),
        typeErrors));
}

#endif